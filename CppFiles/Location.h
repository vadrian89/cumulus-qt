/*
* Copyright (C) 2018 Adrian Verban <verbanady@gmail.com>
* Maintainers: Adrian Verban <verbanady@gmail.com>
* Derived from Typhoon by Archisman Panigrahi which is based on Stormcloud by Jono Cooper <jonocooper.com>
* Thanks to all the contributors.
* Using the Ubuntu Condensed font.
* This file is part of Cumulus.
#
* Cumulus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
#
* Cumulus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
#
* You should have received a copy of the GNU General Public License
* along with Cumulus.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef LOCATION_H
#define LOCATION_H

#include <QObject>
#include <QList>
#include <QThread>
#include <QStringList>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>

#include "DatabaseHelper.h"
class Location : public QObject {
    Q_OBJECT
    int m_locationId;
    QString m_locationCode, m_locationName, m_gpsLocation;
    bool m_isCurrentLocation;
    QList<QObject*> m_locationList;
    QStringList m_locationSearchList;
public:
    explicit Location(QObject *parent = nullptr);
    Q_PROPERTY(int locationId MEMBER m_locationId NOTIFY locationIdChanged)
    Q_PROPERTY(QString locationCode MEMBER m_locationCode NOTIFY locationCodeChanged)
    Q_PROPERTY(QString locationName MEMBER m_locationName NOTIFY locationNameChanged)
    Q_PROPERTY(QList<QObject*> locationList MEMBER m_locationList WRITE setLocationList NOTIFY locationListChanged)
    Q_PROPERTY(bool isCurrentLocation MEMBER m_isCurrentLocation NOTIFY isCurrentLocationChanged)
    Q_PROPERTY(QStringList locationSearchList READ locationSearchList WRITE setLocationSearchList NOTIFY locationSearchListChanged)
    Q_PROPERTY(QString gpsLocation MEMBER m_gpsLocation WRITE setGpsLocation NOTIFY gpsLocationChanged)

    void setLocationList(const QList<QObject*> &locationList);
    QStringList locationSearchList() const;
private slots:
    void readLocationList(const QList<location_struct> &locationList);    
    void locationPositionInfo(const QGeoPositionInfo &posInfo);
public slots:
    void getLocationList();
    void searchLocation(const QString &query);
    void setLocationSearchList(const QStringList &locationList);
    void insertLocation(const QString &location);
    void deleteLocation(const int &locationId);
    void changeCurrentLocation(const int &locationId);
    void getGpsLocation();
    void setGpsLocation(const QString &gpsLocation);
signals:
    void locationListChanged();
    void locationIdChanged();
    void locationCodeChanged();
    void locationNameChanged();
    void isCurrentLocationChanged();
    void noLocation();
    void locationSearchListChanged();
    void duplicateLocation(const QString &location);
    void networkError(const QString &error);
    void gpsLocationChanged();
};

#endif // LOCATION_H
