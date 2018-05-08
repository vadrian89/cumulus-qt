#ifndef LOCATIONCONTROLLER_H
#define LOCATIONCONTROLLER_H

#include <QObject>
#include <QStringList>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "DatabaseHelper.h"
class LocationController : public QObject {
    Q_OBJECT
    QString m_locationName;
    int m_locationId;    
public:
    double m_lon, m_lat;
    explicit LocationController(QObject *parent = nullptr);    
    void setLocationQuery(const QString &locationQuery);
    void setLocationId(const int &locationId);    
private slots:
    void getLocationFromJson(const QJsonObject &jsonObject);
    void getLocationFromYahoo(const QJsonObject &jsonObject);
public slots:
    void getLocationList();
    void searchByLocation();
    void searchByCoordinates();
    void insertLocation();
    void deleteLocation();
signals:
    void locationList(const QList<location_struct> &locationList);
    void locationSearchList(const QStringList &locationList);
    void networkError(const QString &error);
    void duplicateLocation(const QString &location);
    void locationFromGps(const QString &location);
};

#endif // LOCATIONCONTROLLER_H
