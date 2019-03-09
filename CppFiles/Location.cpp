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
#include "Location.h"
#include "LocationController.h"
#include "SettingsController.h"

#include <QGeoCoordinate>

Location::Location(QObject *parent) : QObject(parent) {}

void Location::insertLocation(const QString &location) {
    if (location.trimmed().size() > 0) {
        LocationController controller;
        if (controller.insertLocation(location))
            getLocationList();
    }
}

void Location::deleteLocation(const int &locationId) {
    if (locationId > 0) {
        LocationController controller;
        if (controller.deleteLocation(locationId))
            getLocationList();
    }
}

void Location::getLocationList() {
    QThread *thread = new QThread();
    LocationController *controller = new LocationController();
    controller->moveToThread(thread);
    connect(thread, SIGNAL(started()), controller, SLOT(getLocationList()));
    connect(controller, SIGNAL(locationList(QList<location_struct>)), this, SLOT(readLocationList(QList<location_struct>)));
    connect(thread, SIGNAL(finished()), controller, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(this, SIGNAL(locationListChanged()), thread, SLOT(quit()));
    thread->start();
}

void Location::searchLocation(const QString &query) {
    QThread *thread = new QThread();
    LocationController *controller = new LocationController();
    controller->setLocationQuery(query);
    controller->moveToThread(thread);
    connect(thread, SIGNAL(started()), controller, SLOT(searchByLocation()));
    connect(controller, SIGNAL(locationSearchList(QStringList)), this, SLOT(setLocationSearchList(QStringList)));
    connect(thread, SIGNAL(finished()), controller, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(this, SIGNAL(locationSearchListChanged()), thread, SLOT(quit()));
    thread->start();
}

void Location::readLocationList(const QList<location_struct> &locationList) {
    SettingsController settings;
    if (locationList.isEmpty() && !settings.useGps()) {
        emit noLocation();
    }
    else {        
        QList<QObject*> list;
        for (const location_struct &loc : locationList) {
            Location *location = new Location();
            location->m_locationId = loc.m_locationId;
            location->m_locationCode = loc.m_locationCode;
            location->m_locationName = loc.m_locationName;
            location->m_isCurrentLocation = (settings.currentLocationId() == location->m_locationId);
            list.append(location);
        }
        setLocationList(list);
    }
}

void Location::setLocationList(const QList<QObject*> &locationList) {
    if (m_locationList != locationList) {
        for (QObject *locationObject : m_locationList) {
            delete locationObject;
        }
        m_locationList.clear();
        m_locationList = locationList;
        emit locationListChanged();        
    }
}

void Location::setLocationSearchList(const QStringList &locationList) {
    if (m_locationSearchList != locationList) {
        m_locationSearchList = locationList;
        emit locationSearchListChanged();
    }
}

QStringList Location::locationSearchList() const {
    return m_locationSearchList;
}

void Location::changeCurrentLocation(const int &locationId) {
    SettingsController settings;
    if(!settings.useGps()) {
        settings.setCurrentLocationId(locationId);
        getLocationList();
    }

}

void Location::setGpsLocation(const QString &gpsLocation) {
    if (m_gpsLocation != gpsLocation) {
        m_gpsLocation = gpsLocation;
        emit gpsLocationChanged();
    }
}

void Location::getGpsLocation() {
    QGeoPositionInfoSource *posInfoSource = QGeoPositionInfoSource::createDefaultSource(nullptr);
    if (posInfoSource) {
        QThread *thread = new QThread();
        posInfoSource->setUpdateInterval(2000);
        posInfoSource->moveToThread(thread);
        connect(thread, SIGNAL(started()), posInfoSource, SLOT(startUpdates()));
        connect(posInfoSource, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(locationPositionInfo(QGeoPositionInfo)));
        connect(posInfoSource, SIGNAL(positionUpdated(QGeoPositionInfo)), thread, SLOT(quit()));
        connect(posInfoSource, SIGNAL(error(QGeoPositionInfoSource::Error)), this, SLOT(geoPositionSourceError(QGeoPositionInfoSource::Error)));
        connect(posInfoSource, SIGNAL(error(QGeoPositionInfoSource::Error)), thread, SLOT(quit()));
        connect(thread, SIGNAL(finished()), posInfoSource, SLOT(stopUpdates()));
        connect(thread, SIGNAL(finished()), posInfoSource, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
    }
    else {
        qDebug() << "No position info source available.";
    }
}

void Location::locationPositionInfo(const QGeoPositionInfo &posInfo) {
    LocationController *controller = new LocationController(nullptr);
    QThread *thread = new QThread();
    controller->m_lat = posInfo.coordinate().latitude();
    controller->m_lon = posInfo.coordinate().longitude();
    controller->moveToThread(thread);
    connect(thread, SIGNAL(started()), controller, SLOT(searchByCoordinates()));
    connect(controller, SIGNAL(networkError(QString)), this, SIGNAL(networkError(QString)));
    connect(controller, SIGNAL(locationFromGps(QString)), this, SLOT(setGpsLocation(QString)));
    connect(thread, SIGNAL(finished()), controller, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(this, SIGNAL(networkError(QString)), thread, SLOT(quit()));
    connect(this, SIGNAL(gpsLocationChanged()), thread, SLOT(quit()));
    thread->start();
}

void Location::geoPositionSourceError(QGeoPositionInfoSource::Error positioningError) {
    switch (positioningError) {
    case QGeoPositionInfoSource::AccessError:
        qDebug() << "QGeoPositionInfoSource::AccessError";
        qDebug() << "The connection setup to the remote positioning backend failed because the application lacked the required privileges.";
        break;
    case QGeoPositionInfoSource::ClosedError:
        qDebug() << "QGeoPositionInfoSource::ClosedError";
        qDebug() << "The remote positioning backend closed the connection, which happens for example in case the user is switching location services to off. As soon as the location service is re-enabled regular updates will resume.";
        break;
    case QGeoPositionInfoSource::UnknownSourceError:
        qDebug() << "QGeoPositionInfoSource::UnknownSourceError";
        qDebug() << "An unidentified error occurred.";
        break;
    default:
        break;
    }
}
