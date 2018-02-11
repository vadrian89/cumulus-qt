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

AbstractWeatherController::AbstractWeatherController(QObject *parent) : QObject(parent), operationData(0){
    dataController = new DataController(this);
    db = new DbConnection(this);
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(readJsonData(QJsonObject)));
    connect(dataController, SIGNAL(networkError()), this, SLOT(manageError()));
}

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
    QPointer<DatabaseHelper> dbHelperPtr = new DatabaseHelper;
    bool result = false;
    if (!dbHelperPtr.isNull()) {
        QPointer<Location> locationPtr = dbHelperPtr.data()->getLocation(locationId);
        locationPtr.data()->m_locationCode = code;
        if (!dbHelperPtr.data()->updateLocation(locationPtr))
            emit saveDataError("Error saving location code!");
        else
            result = true;
    }
    return result;
}

void AbstractWeatherController::manageError() {
    emit networkError(dataController->managerError());
}
