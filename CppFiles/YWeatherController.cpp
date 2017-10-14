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
#include "YWeatherController.h"
#include <QRegExp>
#include <QDebug>

YWeatherController::YWeatherController(QObject *parent) : AbstractWeatherController(parent) {
    locationId = 1;
    temperatureUnit = "f";
    locationCode = "";
}

void YWeatherController::searchByLocation(QString &location) {
    operationData = OperationData::GetLocationId;
    locationName = location;
    dataController->getDataFromUrl("https://query.yahooapis.com/v1/public/yql?q=select woeid from geo.places(1) where text='" + location + "'&format=json");
}

void YWeatherController::searchBycode(QString &code) {
    operationData = OperationData::GetWeather;
    dataController->getDataFromUrl("https://query.yahooapis.com/v1/public/yql?q=select * from weather.forecast where woeid=" + code +"&format=json");
}

void YWeatherController::readJsonData(QJsonObject jsonObject) {
    QJsonObject resultsObject = nextBranch(jsonObject, "query").find("results").value().toObject();
    if (operationData == OperationData::GetLocationId) {
        locationCode = nextBranch(resultsObject, "place").find("woeid").value().toString();
        searchBycode(locationCode);
    }
    else {
        weatherObject = jsonObject;
        forecastObject = nextBranch(nextBranch(resultsObject, "channel"), "item");
        emit dataDownloaded();
    }
}

void YWeatherController::saveWeatherToDb(const QJsonObject &jsonObject) {
    qDebug() << "In YWeatherController::saveWeatherToDb";
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
    qint32 weatherCode = -1;
    int temperature = 0;
    QString location = "Undefined";
    int windSpeed = 0;
    int windDegree = 0;
    qint16 humidity = 0;
    QString windSpeedUnit = "mph";
    QString link = "www.google.ro";
    QString description = "";
    QDateTime sunriseTime;
    QDateTime sunsetTime;
    QString pressureUnit = "mbar";
    double pressure = 0.0;

    QJsonObject channel = nextBranch(nextBranch(nextBranch(jsonObject, "query"), "results"), "channel");
    QJsonObject units = nextBranch(channel, "units");
    QJsonObject item = nextBranch(channel, "item");
    QJsonObject wind = nextBranch(channel, "wind");
    QJsonObject atmosphere = nextBranch(channel, "atmosphere");
    QJsonObject astronomy = nextBranch(channel, "astronomy");

    weatherCode = nextBranch(item, "condition").find("code").value().toString().toInt();
    temperature = nextBranch(item, "condition").find("temp").value().toString().toInt();
    description = nextBranch(item, "condition").find("text").value().toString();
    temperatureUnit = units.find("temperature").value().toString().toLower();
    location = nextBranch(channel, "location").find("city").value().toString();
    link = channel.find("link").value().toString().remove(QRegExp("^http.+\\*"));
    windSpeed = wind.find("speed").value().toString().toInt();
    windDegree = wind.find("direction").value().toString().toInt();
    windSpeedUnit = units.find("speed").value().toString().toLower();
    humidity = atmosphere.find("humidity").value().toString().toInt();
    sunriseTime = QLocale().toDateTime(astronomy.find("sunrise").value().toString(), "h:m ap");
    sunsetTime = QLocale().toDateTime(astronomy.find("sunset").value().toString(), "h:m ap");
    pressure = atmosphere.find("pressure").value().toString().toDouble();

    if (clearWeather()) {
        if (db->startCon())  {
            QString str = "insert into sw_tr_weather(w_weather_id, w_weather_code, w_description, "
                          "w_temperature, w_humidity, w_wind_speed, w_wind_degree, w_sunrise, w_sunset, w_loc_id, "
                          "w_pressure, w_link) values (:id, :code, :description, :temperature, "
                          ":humidity, :wind_speed, :wind_degree, :sunrise, :sunset, :loc_id, :pressure, :link)";
            QSqlQuery query;
            query.prepare(str);
            query.bindValue(":id", 1);
            query.bindValue(":code", weatherCode);
            query.bindValue(":description", description);
            query.bindValue(":temperature", Util::calculateTemperature(temperature, temperatureUnit));
            query.bindValue(":humidity", humidity);
            query.bindValue(":wind_speed", Util::calculateWindSpeed(windSpeed, windSpeedUnit));
            query.bindValue(":wind_degree", windDegree);
            query.bindValue(":sunrise", sunriseTime.time().toString(Qt::SystemLocaleShortDate));
            query.bindValue(":sunset", sunsetTime.time().toString(Qt::SystemLocaleShortDate));
            query.bindValue(":pressure", Util::calculatePressure(pressure, pressureUnit));
            query.bindValue(":link", link);
            query.bindValue(":loc_id", 1);
            if (!query.exec()) {
                qDebug() << "YWeatherController::saveWeatherToDb query error: " << query.lastError().text();
                emit saveDataError(query.lastError().text());
            }
        }
        else {
            qDebug() << "YWeatherController::saveWeatherToDb database error: " << db->getError();
            emit saveDataError(db->getError());
        }
        db->stopCon();
    }	
}

void YWeatherController::saveForecastToDb(const QJsonObject &jsonObject) {
    QJsonArray forecastArray = jsonObject.find("forecast").value().toArray();
    qint32 weatherCode = -1;
    qint16 tempHigh = 0;
    qint16 tempLow = 0;
    QLocale yahooLocale(QLocale(QLocale::English));
    QDate date = QDate::currentDate();	
    QString description = "";
    if (clearForecastData()) {
        if (db->startCon())  {
            QString str = "insert into sw_tr_forecast(forec_id, forec_date, forec_weather_code, "
                          "forec_weather_description, forec_temp_min, forec_temp_max, forec_loc_id) "
                          "values "
                          "(:id, :date ,:code, :description, :temp_min, :temp_max, :loc_id)";
            QSqlQuery query;
            query.prepare(str);
            if (db->getDatabase().driver()->hasFeature(QSqlDriver::Transactions)) {
                db->getDatabase().transaction();
            }
		    for ( int i = 0; i < forecastArray.size(); i++) {
                weatherCode = forecastArray.at(i).toObject().find("code").value().toString().toInt();
                tempHigh = forecastArray.at(i).toObject().find("high").value().toString().toInt();
                tempLow = forecastArray.at(i).toObject().find("low").value().toString().toInt();
				QString forecastDateStr = forecastArray.at(i).toObject().find("date").value().toString().replace(" ", "/");
				date = yahooLocale.toDate(forecastDateStr, "dd/MMM/yyyy");
                description = forecastArray.at(i).toObject().find("text").value().toString();
                query.bindValue(":id", i + 1);
                query.bindValue(":date", date);
                query.bindValue(":code", weatherCode);
                query.bindValue(":description", description);
                query.bindValue(":temp_max", Util::calculateTemperature(tempHigh, temperatureUnit));
                query.bindValue(":temp_min", Util::calculateTemperature(tempLow, temperatureUnit));
                query.bindValue(":loc_id", locationId);
                if ( !query.exec() ) {
                    qDebug() << "YWeatherController::saveForecastToDb query error: " << query.lastError().text();
                    qDebug() << "YWeatherController::saveForecastToDb db error: " << db->getError();
                    emit saveDataError(query.lastError().text());
                    if ( db->getDatabase().driver()->hasFeature(QSqlDriver::Transactions)) {
                        db->getDatabase().rollback();
                    }
                    break;
                }
            }
            if ( db->getDatabase().driver()->hasFeature(QSqlDriver::Transactions)) {
                db->getDatabase().commit();
            }
            emit forecastChanged();
        }
        else {
            qDebug() << "YWeatherController::saveForecastToDb database error: " << db->getError();
            emit saveDataError(db->getError());
        }
        db->stopCon();
    }
}
