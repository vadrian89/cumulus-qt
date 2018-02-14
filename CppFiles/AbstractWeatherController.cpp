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

AbstractWeatherController::AbstractWeatherController(QObject *parent) : QObject(parent),
    operationData(0) {}

QJsonObject AbstractWeatherController::nextBranch(const QJsonObject &jsonObject, const QString current) const {
    return jsonObject.find(current).value().toObject();
}

void AbstractWeatherController::saveDataToDb() {
    if (locationCode.trimmed().size() > 0)
        saveLocation(locationCode);
    saveWeatherToDb(weatherObject);
    saveForecastToDb(forecastObject);
}

bool AbstractWeatherController::saveLocation(const QString &code) {
    unique_ptr<DatabaseHelper> dbHelperPtr(new DatabaseHelper);
    bool result = false;
    if (dbHelperPtr) {
        unique_ptr<Location> locationPtr(dbHelperPtr.get()->getLocation(locationId));
        locationPtr.get()->m_locationCode = code;
        if (!dbHelperPtr.get()->updateLocation(locationPtr.get()))
            emit saveDataError("Error saving location code!");
        else
            result = true;
    }
    return result;
}

void AbstractWeatherController::manageError(const QString &error) {
    emit networkError(error);
}

void AbstractWeatherController::setWeather() {
    SettingsController settings;
    DatabaseHelper dbHelper;
    const unique_ptr<Weather> weather(dbHelper.getWeather(settings.currentLocationId()));
    if (weather)
        emit weatherReady(weather.get());
}

void AbstractWeatherController::getWeather() {
    SettingsController settings;
    DatabaseHelper dbHelper;
    dataController = new DataController(this);
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(readJsonData(QJsonObject)));
    connect(dataController, SIGNAL(networkError(QString)), this, SLOT(manageError(QString)));
    connect(this, SIGNAL(dataDownloaded()), this, SLOT(saveDataToDb()));
    connect(this, SIGNAL(forecastChanged()), this, SLOT(setWeather()));
    int locationId = settings.currentLocationId();
    unique_ptr<Location> locationPtr(dbHelper.getLocation(locationId));
    if (locationPtr) {
        if (locationPtr.get()->m_locationCode.trimmed().size() > 0) {
            searchBycode(locationPtr.get()->m_locationCode);
        }
        else {
            searchByLocation(locationPtr.get()->m_locationName);
        }
    }
}
