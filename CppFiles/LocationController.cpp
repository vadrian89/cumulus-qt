#include "LocationController.h"
#include "DataController.h"
LocationController::LocationController(QObject *parent) : QObject(parent) {}

void LocationController::setLocationQuery(const QString &locationName) {
    if (m_locationName != locationName) {
        m_locationName = locationName;
    }
}

void LocationController::setLocationId(const int &locationId) {
    if (m_locationId != locationId) {
        m_locationId = locationId;
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
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getLocationFromJson(QJsonObject)));
    dataController->getDataFromUrl("http://autocomplete.wunderground.com/aq?query=" + m_locationName);
}

void LocationController::searchByCoordinates() {
    DataController *dataController = new DataController(this);
    connect(dataController, SIGNAL(networkError(QString)), this, SIGNAL(networkError(QString)));
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getLocationFromYahoo(QJsonObject)));
    dataController->getDataFromUrl("https://query.yahooapis.com/v1/public/yql?q=select * from geo.places WHERE text=\"(" + QString::number(m_lat) + "," + QString::number(m_lon) + ")\"&format=json");
}

void LocationController::getLocationFromYahoo(const QJsonObject &jsonObject) {
    QJsonObject result = jsonObject.find("query").value().toObject()
            .find("results").value().toObject()
            .find("place").value().toObject();
    QString city = result.find("locality1").value().toObject().find("content").value().toString();
    QString country = result.find("country").value().toObject().find("content").value().toString();
    QString gpsCity = city.append(", ").append(country);
    emit locationFromGps(gpsCity);
}

void LocationController::getLocationFromJson(const QJsonObject &jsonObject) {
    QJsonArray array = jsonObject.find("RESULTS").value().toArray();
    QStringList stringList;
    for (int i = 0; i < array.size(); i++) {
        stringList.append(array.at(i).toObject().find("name").value().toString());
    }
    emit locationSearchList(stringList);
}

void LocationController::insertLocation() {
    DatabaseHelper dbHelper;
    location_struct loc = dbHelper.findLocation(this->m_locationName);
    if (loc.m_locationName.trimmed().toLower() == this->m_locationName.trimmed().toLower()) {
        emit duplicateLocation(this->m_locationName);
    }
    else {
        location_struct location;
        location.m_locationCode = "";
        location.m_locationName = this->m_locationName;
        if (dbHelper.insertLocation(location)) {
            getLocationList();
        }
    }
}

void LocationController::deleteLocation() {
    DatabaseHelper dbHelper;
    if (dbHelper.deleteLocation(m_locationId))
        getLocationList();
}
