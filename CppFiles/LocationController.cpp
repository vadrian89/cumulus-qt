#include "LocationController.h"
#include "DataController.h"
LocationController::LocationController(QObject *parent) : QObject(parent) {}

void LocationController::setLocationQuery(const QString &locationName) {
    if (m_locationName != locationName) {
        m_locationName = locationName;
    }
}

void LocationController::getLocationList() {
    DatabaseHelper dbHelper;
    QList<location_struct> list;
    list = dbHelper.getLocationList();
    emit locationList(list);
}

void LocationController::searchByLocation() {
    DataController *dataController = new DataController(this);
    connect(dataController, SIGNAL(networkError(QString)), this, SIGNAL(networkError(QString)));
    connect(dataController, SIGNAL(jsonArrayReady(QJsonArray)), this, SLOT(getLocationFromJson(QJsonArray)));
    dataController->getJsonArrayDataFromUrl("https://api.locationiq.com/v1/autocomplete.php?key=91832c9892b3cc&q=" + m_locationName);
}

void LocationController::searchByCoordinates() {
    DataController *dataController = new DataController(this);
    connect(dataController, SIGNAL(networkError(QString)), this, SIGNAL(networkError(QString)));
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getGeoLocation(QJsonObject)));
    dataController->getDataFromUrl("https://eu1.locationiq.com/v1/reverse.php?key=91832c9892b3cc&lat=" + QString::number(m_lat) + "&lon=" + QString::number(m_lon) + "&format=json");
}

void LocationController::getGeoLocation(const QJsonObject &jsonObject) {
    QJsonObject result = jsonObject.find("address").value().toObject();
    QString city = result.find("city").value().toString();
    if (city.trimmed().length() == 0)
        city = result.find("city_district").value().toString();
    if (city.trimmed().length() == 0)
        city = result.find("locality").value().toString();
    if (city.trimmed().length() == 0)
        city = result.find("town").value().toString();
    if (city.trimmed().length() == 0)
        city = result.find("borough").value().toString();
    if (city.trimmed().length() == 0)
        city = result.find("municipality").value().toString();
    if (city.trimmed().length() == 0)
        city = result.find("village").value().toString();
    if (city.trimmed().length() == 0)
        city = result.find("hamlet").value().toString();
    if (city.trimmed().length() == 0)
        city = result.find("quarter").value().toString();
    if (city.trimmed().length() == 0)
        city = result.find("neighbourhood").value().toString();
    QString country = result.find("country").value().toString();
    QString gpsCity = city.append(", ").append(country);
    emit locationFromGps(gpsCity);
}

void LocationController::getLocationFromJson(const QJsonArray &jsonArray) {
    QStringList stringList;
    for (int i = 0; i < jsonArray.size(); i++) {
        stringList.append(jsonArray.at(i).toObject().find("display_name").value().toString());
    }
    emit locationSearchList(stringList);
}

bool LocationController::insertLocation(const QString &locationName) {
    DatabaseHelper dbHelper;
    location_struct loc = dbHelper.findLocation(locationName);
    if (loc.m_locationId > 0) {
        return false;
    }
    else {
        loc.m_locationCode = "";
        loc.m_locationName = locationName;
        return dbHelper.insertLocation(loc);
    }
}

bool LocationController::deleteLocation(const int &locationId) {
    DatabaseHelper dbHelper;
    return dbHelper.deleteLocation(locationId);
}
