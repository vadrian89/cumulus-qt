/*
* Copyright (C) 2016 - 2017 Adrian Verban <verbanady@gmail.com>
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
#include "AbstractWeatherController.h"

AbstractWeatherController::AbstractWeatherController(QObject *parent) : QObject(parent)
  , name("abstract") {}

QJsonObject AbstractWeatherController::nextBranch(const QJsonObject &jsonObject, const QString current) const {
    return jsonObject.find(current).value().toObject();
}

void AbstractWeatherController::manageError(const QString error) {
    qDebug() << "AbstractWeatherController::manageError" << error;
    emit networkError(error);
}

void AbstractWeatherController::getWeather() {
    SettingsController settings;    
    dataController = new DataController(this);
    connect(dataController, SIGNAL(networkError(QString)), this, SLOT(manageError(QString)));
    if (settings.useGps()) {
        QGeoPositionInfoSource *posInfoSource = QGeoPositionInfoSource::createDefaultSource(this);
        if (posInfoSource) {
            posInfoSource->setUpdateInterval(10000);
            connect(posInfoSource, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(locationPositionInfo(QGeoPositionInfo)));
            connect(posInfoSource, SIGNAL(positionUpdated(QGeoPositionInfo)), posInfoSource, SLOT(stopUpdates()));
            connect(posInfoSource, SIGNAL(positionUpdated(QGeoPositionInfo)), posInfoSource, SLOT(deleteLater()));
            connect(posInfoSource, SIGNAL(error(QGeoPositionInfoSource::Error)), this, SLOT(locationPositionError(QGeoPositionInfoSource::Error)));
            posInfoSource->startUpdates();
        }
        else {
            emit networkError("Could not use GPS!");
        }
    }
    else {
        DatabaseHelper dbHelper;
        location_struct location = dbHelper.getLocation(settings.currentLocationId());
        if (location.m_locationId > 0) {
            if (location.m_locationCode.trimmed().size() > 0) {
                searchBycode(location.m_locationCode);
            }
            else {
                searchByLocation(location.m_locationName);
            }
        }
    }
}

bool AbstractWeatherController::saveLocation(const QString &code) {
    SettingsController settings;
    bool result = true;
    if (!settings.useGps()) {
        result = false;
        DatabaseHelper dbHelper;
        location_struct location = dbHelper.getLocation(settings.currentLocationId());
        location.m_locationCode = code;
        result = dbHelper.updateLocation(location);
        if (!result)
            emit saveDataError("Error saving location code!");
    }
    return result;
}

bool AbstractWeatherController::saveLocation(const QString &code, const QString &name) {
    SettingsController settings;
    DatabaseHelper dbHelper;
    bool result = false;
    location_struct location = dbHelper.getLocation(settings.currentLocationId());
    location.m_locationCode = code;
    location.m_locationName = name;
    result = dbHelper.updateLocation(location);
    if (!result)
        emit saveDataError("Error saving location code!");
    return result;
}

bool AbstractWeatherController::saveWeather(const weather_struct &weather) {
    DatabaseHelper dbHelper;
    bool result = dbHelper.deleteWeather(weather.m_locationId);
    if (result) {
        result = dbHelper.insertWeather(weather);
        if (!result)
            emit saveDataError("Error saving weather!");
    }
    return result;
}

void AbstractWeatherController::saveForecast(QList<Forecast*> &forecastList) {
    DatabaseHelper dbHelper;
    bool result = dbHelper.insertForecast(forecastList);    
    m_weather.m_forecastListPtr = new QList<Forecast*>(forecastList);
    if (!result)
        emit saveDataError("Error saving forecast!");
    else
        emit weatherSet(m_weather);
}

void AbstractWeatherController::locationPositionInfo(const QGeoPositionInfo &posInfo) {
    double lat = posInfo.coordinate().latitude();
    double lon = posInfo.coordinate().longitude();
    searchByGps(lat, lon);
}

void AbstractWeatherController::locationPositionError(const QGeoPositionInfoSource::Error &positioningError) {
    switch (positioningError) {
    case QGeoPositionInfoSource::AccessError:
        manageError("Location has access error!");
        break;
    case QGeoPositionInfoSource::ClosedError:
        manageError("Location services are off!");
        break;
    case QGeoPositionInfoSource::UnknownSourceError:
        manageError("An unidentified error occurred when requesting loation!");
        break;
    default:
        break;
    }
}
