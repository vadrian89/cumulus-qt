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

QJsonObject AbstractWeatherController::nextBranch(const QJsonObject jsonObject, const QString current) const {
    return jsonObject.find(current).value().toObject();
}

void AbstractWeatherController::saveDataToDb() {
    if (locationCode.trimmed().size() > 0)
        saveLocation(locationCode, locationName);
    saveWeatherToDb(weatherObject);
    saveForecastToDb(forecastObject);
}

bool AbstractWeatherController::saveLocation(const QString &code, const QString &location) {
    if (db->startCon() == true) {
        QString queryString = "update sw_ma_location set loc_code = :code, loc_name = :location "
                              "where loc_id = :id";
        QSqlQuery query;
        query.prepare(queryString);
        query.bindValue(":code", code);
        query.bindValue(":id", locationId);
        query.bindValue(":location", location);
        if (query.exec()) {
            return true;
        }
        else {
            qDebug() << "AbstractWeatherController::saveLocationCode query error: " << query.lastError().text();
            emit saveDataError(query.lastError().text());
            return false;
        }
    }
    else {
        qDebug() << "AbstractWeatherController::saveLocationCode database error: " << db->getError();
        emit saveDataError(db->getError());
        return false;
    }
    db->stopCon();
}

bool AbstractWeatherController::clearWeather() {
    if (db->startCon())  {
        QString str = "delete from sw_tr_weather where w_loc_id = :id";
        QSqlQuery query;
        query.prepare(str);
        query.bindValue(":id", locationId);
        if (query.exec()) {
            return true;
        }
        else {
            qDebug() << "AbstractWeatherController::clearWeather query error: " << query.lastError().text();
            return false;
        }
    }
    else {
        qDebug() << "AbstractWeatherController::clearWeather database error: " << db->getError();
        return false;
    }
    db->stopCon();
}

bool AbstractWeatherController::clearForecastData() {
    if (db->startCon())  {
        QString str = "delete from sw_tr_forecast where forec_loc_id = :id";
        QSqlQuery query;
        query.prepare(str);
        query.bindValue(":id", locationId);
        if ( query.exec() ) {
            return true;
        }
        else {
            qDebug() << "AbstractWeatherController::clearForecastData query error: " << query.lastError().text();
            return false;
        }
    }
    else {
        qDebug() << "AbstractWeatherController::clearForecastData database error: " << db->getError();
        return false;
    }

    db->stopCon();
}

void AbstractWeatherController::manageError() {
    emit networkError(dataController->managerError());
}
