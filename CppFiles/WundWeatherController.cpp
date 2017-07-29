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
    temperatureUnit = "k";
    locationCode = "";
}

void WundWeatherController::searchByLocation(QString &location) {
    operationData = OperationData::GetLocationId;
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toLower();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.openweathermap.org/data/2.5/weather?q=" + location + "&appid=" + apiKey + "&lang=" + systemLang);
}

void WundWeatherController::searchBycode(QString &code) {
    operationData = OperationData::GetWeather;
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toLower();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.openweathermap.org/data/2.5/weather?id=" + code + "&appid=" + apiKey + "&lang=" + systemLang);
}

void WundWeatherController::getForecast(const QString &code) {
    operationData = OperationData::GetForecast;
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toLower();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.openweathermap.org/data/2.5/forecast/daily?id=" + code + "&appid=" + apiKey + "&lang=" + systemLang);
}

void WundWeatherController::readJsonData(QJsonObject jsonObject) {
//    if (operationData == OperationData::GetLocationId || operationData == OperationData::GetWeather) {
//        locationCode = QString::number(jsonObject.find("id").value().toInt());
//        locationName = jsonObject.find("name").value().toString();
//        weatherObject = jsonObject;
//        getForecast(locationCode);
//    }
//    else if (operationData == OperationData::GetForecast) {
//        forecastObject = jsonObject;
//        emit dataDownloaded();
//    }
}

void WundWeatherController::saveWeatherToDb(const QJsonObject jsonObject) {
    qDebug() << "In WundWeatherController::saveWeatherToDb";
//    qint32 weatherCode = -1;
//    float temperature = 0;
//    int windSpeed = 0;
//    int windDegree = 0;
//    qint16 humidity = 0;
//    QString windSpeedUnit = "m/s";
//    QString description = "";
//    QDateTime sunriseTime;
//    QDateTime sunsetTime;
//    QString pressureUnit = "mbar";
//    QString link = "http://openweathermap.org/city/" + locationCode;
//    double pressure = 0.0;

//    QJsonObject weatherData = jsonObject.find("weather").value().toArray().at(0).toObject();

//    weatherCode = weatherData.find("id").value().toInt();
//    temperature = nextBranch(jsonObject,"main").find("temp").value().toDouble();
//    description = weatherData.find("description").value().toString();
//    windSpeed = nextBranch(jsonObject,"wind").find("speed").value().toDouble();
//    windDegree = nextBranch(jsonObject,"wind").find("deg").value().toInt();
//    humidity = nextBranch(jsonObject,"main").find("humidity").value().toInt();
//    sunriseTime = QDateTime::fromTime_t(nextBranch(jsonObject,"sys").find("sunrise").value().toInt());
//    sunsetTime = QDateTime::fromTime_t(nextBranch(jsonObject,"sys").find("sunset").value().toInt());
//    pressure = nextBranch(jsonObject,"main").find("pressure").value().toDouble();
//    if (clearWeather()) {
//        if (db->startCon())  {
//            QString str = "insert into sw_tr_weather(w_weather_id, w_weather_code, w_description, "
//                          "w_temperature, w_humidity, w_wind_speed, w_wind_degree, w_sunrise, w_sunset, w_loc_id, "
//                          "w_pressure, w_link) values (:id, :code, :description, :temperature, "
//                          ":humidity, :wind_speed, :wind_degree, :sunrise, :sunset, :loc_id, :pressure, :link)";
//            QSqlQuery query;
//            query.prepare(str);
//            query.bindValue(":id", 1);
//            query.bindValue(":code", weatherCode);
//            query.bindValue(":description", description);
//            query.bindValue(":temperature", Util::calculateTemperature(temperature, temperatureUnit));
//            query.bindValue(":humidity", humidity);
//            query.bindValue(":wind_speed", Util::calculateWindSpeed(windSpeed, windSpeedUnit));
//            query.bindValue(":wind_degree", windDegree);
//            query.bindValue(":sunrise", sunriseTime.time().toString(Qt::SystemLocaleShortDate));
//            query.bindValue(":sunset", sunsetTime.time().toString(Qt::SystemLocaleShortDate));
//            query.bindValue(":pressure", Util::calculatePressure(pressure, pressureUnit));
//            query.bindValue(":link", link);
//            query.bindValue(":loc_id", locationId);
//            if (!query.exec()) {
//                qDebug() << "WundWeatherController::saveWeatherToDb query error: " << query.lastError().text();
//                emit saveDataError(query.lastError().text());
//            }
//        }
//        else {
//            qDebug() << "WundWeatherController::saveWeatherToDb database error: " << db->getError();
//            emit saveDataError(db->getError());
//        }
//        db->stopCon();
//    }
}

void WundWeatherController::saveForecastToDb(const QJsonObject jsonObject) {
//    QJsonArray forecastArray = jsonObject.find("list").value().toArray();
//    qint32 weatherCode = -1;
//    qint16 tempHigh = 0;
//    qint16 tempLow = 0;
//    QDate date = QDate::currentDate();
//    QString description = "";
//    if (clearForecastData()) {
//        if (db->startCon()) {
//            QString str = "insert into sw_tr_forecast(forec_id, forec_date, forec_weather_code, "
//                          "forec_weather_description, forec_temp_min, forec_temp_max, forec_loc_id) "
//                          "values "
//                          "(:id, :date ,:code, :description, :temp_min, :temp_max, :loc_id)";
//            QSqlQuery query;
//            query.prepare(str);
//            if ( db->getDatabase().driver()->hasFeature(QSqlDriver::Transactions)) {
//                db->getDatabase().transaction();
//            }
//            for (int i = 0; i < forecastArray.size(); i++) {
//                QJsonObject weatherInfo = forecastArray.at(i).toObject().find("weather").value().toArray().at(0).toObject();
//                weatherCode = weatherInfo.find("id").value().toInt();
//                tempHigh = nextBranch(forecastArray.at(i).toObject(), "temp").find("day").value().toDouble();
//                tempLow = nextBranch(forecastArray.at(i).toObject(), "temp").find("night").value().toDouble();
//                date = QDate::fromString(QDateTime::fromTime_t(forecastArray.at(i).toObject().find("dt").value().toInt()).toString("dd/MM/yyyy"), "dd/MM/yyyy");
//                description = weatherInfo.find("description").value().toString();
//                query.bindValue(":id", i + 1);
//                query.bindValue(":date", date);
//                query.bindValue(":code", weatherCode);
//                query.bindValue(":description", description);
//                query.bindValue(":temp_max", Util::calculateTemperature(tempHigh, temperatureUnit));
//                query.bindValue(":temp_min", Util::calculateTemperature(tempLow, temperatureUnit));
//                query.bindValue(":loc_id", locationId);
//                if (!query.exec()) {
//                    qDebug() << "WundWeatherController::saveForecastToDb query error: " << query.lastError().text();
//                    qDebug() << "WundWeatherController::saveForecastToDb db error: " << db->getError();
//                    emit saveDataError(query.lastError().text());
//                    if (db->getDatabase().driver()->hasFeature(QSqlDriver::Transactions)) {
//                        db->getDatabase().rollback();
//                    }
//                    break;
//                }
//            }
//            if (db->getDatabase().driver()->hasFeature(QSqlDriver::Transactions)) {
//                db->getDatabase().commit();
//            }
//            emit forecastChanged();
//        }
//        else {
//            qDebug() << "WundWeatherController::saveForecastToDb database error: " << db->getError();
//            emit saveDataError(db->getError());
//        }
//        db->stopCon();
//    }
}
