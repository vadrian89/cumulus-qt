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
#include "WundWeatherController.h"

WundWeatherController::WundWeatherController(QObject *parent) : AbstractWeatherController(parent) {
    apiKey = "30be6723cf95f92c";
    locationId = 1;
    temperatureUnit = "c";
    locationCode = "";
    wundIconsCodes = new QMap<QString, int>();
    mapWundIconsCodes();
}

void WundWeatherController::searchByLocation(QString &location) {
    operationData = OperationData::GetLocationId;
    dataController->getDataFromUrl("http://autocomplete.wunderground.com/aq?query=" + location);
}

void WundWeatherController::searchBycode(QString &code) {
    operationData = OperationData::GetWeather;
    locationCode = code;
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toLower();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.wunderground.com/api/" + apiKey + "/conditions/lang:RO/q/zmw:" + code + ".json");
}

void WundWeatherController::getAstronomy(const QString &code) {
    operationData = OperationData::GetAstronomy;
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toLower();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.wunderground.com/api/" + apiKey + "/astronomy/lang:RO/q/zmw:" + code + ".json");
}

void WundWeatherController::getForecast(const QString &code) {
    operationData = OperationData::GetForecast;
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toLower();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.wunderground.com/api/" + apiKey + "/forecast10day/lang:RO/q/zmw:" + code + ".json");
}

void WundWeatherController::readJsonData(QJsonObject jsonObject) {
    if (operationData == OperationData::GetLocationId) {
        QJsonArray array = jsonObject.find("RESULTS").value().toArray();
        locationCode = array.at(0).toObject().find("zmw").value().toString();
        searchBycode(locationCode);
    }
    else if (operationData == OperationData::GetWeather) {
        weatherObject = jsonObject;
        getAstronomy(locationCode);
    }
    else if (operationData == OperationData::GetAstronomy) {
        QJsonObject sunriseData = nextBranch(nextBranch(jsonObject, "sun_phase"), "sunrise");
        QJsonObject sunsetData = nextBranch(nextBranch(jsonObject, "sun_phase"), "sunset");
        sunrise = QDateTime::fromString(sunriseData.find("hour").value().toString() + ":" +
                sunriseData.find("minute").value().toString(), "h:mm");
        sunset = QDateTime::fromString(sunsetData.find("hour").value().toString() + ":" +
                sunsetData.find("minute").value().toString(), "h:mm");
        getForecast(locationCode);
    }
    else {
        forecastObject = jsonObject;
        emit dataDownloaded();
    }
}

void WundWeatherController::saveWeatherToDb(const QJsonObject &jsonObject) {
    qDebug() << "In WundWeatherController::saveWeatherToDb";
    int weatherCode = -1;
    float temperature = 0;
    int windSpeed = 0;
    int windDegree = 0;
    int humidity = 0;
    QString windSpeedUnit = "kph";
    QString description = "";
    QString pressureUnit = "mbar";
    QString link = "http://www.wunderground.com/q/zmw:" + locationCode;
    double pressure = 0.0;

    QJsonObject weatherData = nextBranch(jsonObject, "current_observation");    

    weatherCode = wundIconsCodes->find(weatherData.find("icon").value().toString()).value();
    temperature = weatherData.find("temp_c").value().toDouble();
    description = weatherData.find("weather").value().toString();
    windSpeed = weatherData.find("wind_kph").value().toDouble();
    windDegree = weatherData.find("wind_degrees").value().toInt();
    humidity = weatherData.find("relative_humidity").value().toString().remove("%").toInt();
    pressure = weatherData.find("pressure_mb").value().toString().toDouble();
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
            query.bindValue(":sunrise", sunrise.time().toString(Qt::SystemLocaleShortDate));
            query.bindValue(":sunset", sunset.time().toString(Qt::SystemLocaleShortDate));
            query.bindValue(":pressure", Util::calculatePressure(pressure, pressureUnit));
            query.bindValue(":link", link);
            query.bindValue(":loc_id", locationId);
            if (!query.exec()) {
                qDebug() << "WundWeatherController::saveWeatherToDb query error: " << query.lastError().text();
                emit saveDataError(query.lastError().text());
            }
        }
        else {
            qDebug() << "WundWeatherController::saveWeatherToDb database error: " << db->getError();
            emit saveDataError(db->getError());
        }
        db->stopCon();
    }
}

void WundWeatherController::saveForecastToDb(const QJsonObject &jsonObject) {
    QJsonArray forecastArray = nextBranch(nextBranch(jsonObject, "forecast"), "simpleforecast").find("forecastday").value().toArray();
    int weatherCode = -1;
    int tempHigh = 0;
    int tempLow = 0;
    QDate date = QDate::currentDate();
    QString description = "";
    int idCounter = 0;
    if (clearForecastData()) {
        if (db->startCon()) {
            QString str = "insert into sw_tr_forecast(forec_id, forec_date, forec_weather_code, "
                          "forec_weather_description, forec_temp_min, forec_temp_max, forec_loc_id) "
                          "values "
                          "(:id, :date ,:code, :description, :temp_min, :temp_max, :loc_id)";
            QSqlQuery query;
            query.prepare(str);
            if ( db->getDatabase().driver()->hasFeature(QSqlDriver::Transactions)) {
                db->getDatabase().transaction();
            }
            foreach (QJsonValue val, forecastArray) {
                if (wundIconsCodes->constFind(val.toObject().find("icon").value().toString()) != wundIconsCodes->constEnd())
                    weatherCode = wundIconsCodes->constFind(val.toObject().find("icon").value().toString()).value();
                tempHigh = nextBranch(val.toObject(), "high").find("celsius").value().toString().toInt();
                tempLow = nextBranch(val.toObject(), "low").find("celsius").value().toString().toInt();
                date = dateFromJson(nextBranch(val.toObject(), "date"));
                description = val.toObject().find("conditions").value().toString();
                query.bindValue(":id", ++idCounter);
                query.bindValue(":date", date);
                query.bindValue(":code", weatherCode);
                query.bindValue(":description", description);
                query.bindValue(":temp_max", Util::calculateTemperature(tempHigh, temperatureUnit));
                query.bindValue(":temp_min", Util::calculateTemperature(tempLow, temperatureUnit));
                query.bindValue(":loc_id", locationId);                
                if (!query.exec()) {
                    qDebug() << "WundWeatherController::saveForecastToDb query error: " << query.lastError().text();
                    qDebug() << "WundWeatherController::saveForecastToDb db error: " << db->getError();
                    emit saveDataError(query.lastError().text());
                    if (db->getDatabase().driver()->hasFeature(QSqlDriver::Transactions)) {
                        db->getDatabase().rollback();
                    }
                    break;
                }
            }
            if (db->getDatabase().driver()->hasFeature(QSqlDriver::Transactions)) {
                db->getDatabase().commit();
            }
            emit forecastChanged();
        }
        else {
            qDebug() << "WundWeatherController::saveForecastToDb database error: " << db->getError();
            emit saveDataError(db->getError());
        }
        db->stopCon();
    }
}

void WundWeatherController::mapWundIconsCodes() {
    wundIconsCodes->insert("chanceflurries", 1);
    wundIconsCodes->insert("chancerain", 2);
    wundIconsCodes->insert("chancesleet", 3);
    wundIconsCodes->insert("chancesnow", 4);
    wundIconsCodes->insert("chancetstorms", 5);
    wundIconsCodes->insert("clear", 6);
    wundIconsCodes->insert("cloudy", 7);
    wundIconsCodes->insert("flurries", 8);
    wundIconsCodes->insert("fog", 9);
    wundIconsCodes->insert("hazy", 10);
    wundIconsCodes->insert("mostlycloudy", 11);
    wundIconsCodes->insert("mostlysunny", 12);
    wundIconsCodes->insert("partlycloudy", 13);
    wundIconsCodes->insert("partlysunny", 14);
    wundIconsCodes->insert("sleet", 15);
    wundIconsCodes->insert("rain", 16);
    wundIconsCodes->insert("snow", 17);
    wundIconsCodes->insert("sunny", 18);
    wundIconsCodes->insert("tstorms", 19);
    wundIconsCodes->insert("unknown", 20);
    wundIconsCodes->insert("-1", -1);
}

QDate WundWeatherController::dateFromJson(const QJsonObject &jsonObject) {    
    QDate date = QDate::fromString(QString::number(jsonObject.find("day").value().toInt())
                                   + QString::number(jsonObject.find("month").value().toInt())
                                   + QString::number(jsonObject.find("year").value().toInt()), "dMyyyy");
//    qDebug() << QString::number(jsonObject.find("day").value().toInt())
//             << QString::number(jsonObject.find("month").value().toInt())
//             << QString::number(jsonObject.find("year").value().toInt()) << endl;
//    qDebug() << date << endl;
    return date;
}
