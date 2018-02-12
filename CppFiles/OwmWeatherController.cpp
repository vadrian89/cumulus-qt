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
#include "OwmWeatherController.h"

OwmWeatherController::OwmWeatherController(QObject *parent) : AbstractWeatherController(parent) {
    apiKey = "15c2836a71c126524d35af1d29d563c5";
    locationId = 1;
    temperatureUnit = "k";
    locationCode = "";
}

void OwmWeatherController::searchByLocation(QString &location) {
    operationData = OperationData::GetLocationId;
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toLower();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.openweathermap.org/data/2.5/weather?q=" + location + "&appid=" + apiKey + "&lang=" + systemLang);
    locationName = location;
}

void OwmWeatherController::searchBycode(QString &code) {
    operationData = OperationData::GetWeather;
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toLower();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.openweathermap.org/data/2.5/weather?id=" + code + "&appid=" + apiKey + "&lang=" + systemLang);
}

void OwmWeatherController::getForecast(const QString &code) {
    operationData = OperationData::GetForecast;
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toLower();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.openweathermap.org/data/2.5/forecast/daily?id=" + code + "&appid=" + apiKey + "&lang=" + systemLang);
}

void OwmWeatherController::readJsonData(QJsonObject jsonObject) {
    if (operationData == OperationData::GetLocationId || operationData == OperationData::GetWeather) {
        locationCode = QString::number(jsonObject.find("id").value().toInt());
        weatherObject = jsonObject;
        getForecast(locationCode);
    }
    else if (operationData == OperationData::GetForecast) {
        forecastObject = jsonObject;
        emit dataDownloaded();
    }
}

void OwmWeatherController::saveWeatherToDb(const QJsonObject &jsonObject) {
    qDebug() << "In OwmWeatherController::saveWeatherToDb";
    unique_ptr<DatabaseHelper> dbHelperPtr(new DatabaseHelper);
    if (dbHelperPtr) {
        unique_ptr<Weather> weatherPtr(getWeatherFromJson(jsonObject));
        if (weatherPtr && dbHelperPtr.get()->deleteWeather(locationId)) {
            if (!dbHelperPtr.get()->insertWeather(weatherPtr.get())) {
                qDebug() << "OwmWeatherController::saveWeatherToDb error!";
                emit saveDataError("Error saving the weather to database!");
            }
        }
    }
}

void OwmWeatherController::saveForecastToDb(const QJsonObject &jsonObject) {
    QJsonArray forecastArray = jsonObject.find("list").value().toArray();
    int weatherCode = -1;
    int tempHigh = 0;
    int tempLow = 0;
    QDate date = QDate::currentDate();
    QString description = "";
    QList<Forecast*> forecastList;
    for (QJsonValue forecastJson : forecastArray) {
        Forecast *forecast = new Forecast();
        QJsonObject weatherInfo = forecastJson.toObject().find("weather").value().toArray().at(0).toObject();
        weatherCode = weatherInfo.find("id").value().toInt();
        tempHigh = nextBranch(forecastJson.toObject(), "temp").find("day").value().toDouble();
        tempLow = nextBranch(forecastJson.toObject(), "temp").find("night").value().toDouble();
        date = QDate::fromString(QDateTime::fromTime_t(forecastJson.toObject().find("dt").value().toInt()).toString("dd/MM/yyyy"), "dd/MM/yyyy");
        description = weatherInfo.find("description").value().toString();
        forecast->setWeatherCode(weatherCode);
        forecast->setTempLow(Util::calculateTemperature(tempLow, temperatureUnit));
        forecast->setTempHigh(Util::calculateTemperature(tempHigh, temperatureUnit));
        forecast->setForecastDesc(description);
        forecast->setForecastDate(date.toString("dd/MMM/yyyy"));
        forecast->setLocationId(locationId);
        forecastList.append(forecast);
    }
    unique_ptr<DatabaseHelper> dbHelperPtr(new DatabaseHelper);
    if (dbHelperPtr.get()->insertForecast(forecastList))
        emit forecastChanged();
    else
        emit saveDataError("Error saving forecast!");
}

Weather* OwmWeatherController::getWeatherFromJson(const QJsonObject &jsonObject) {
    Weather *weatherPtr = nullptr;
    if (!jsonObject.isEmpty()) {
        weatherPtr = new Weather;
        int weatherCode = -1;
        float temperature = 0;
        int windSpeed = 0;
        int windDegree = 0;
        int humidity = 0;
        QString windSpeedUnit = "m/s";
        QString description = "";
        QDateTime sunriseTime;
        QDateTime sunsetTime;
        QString pressureUnit = "mbar";
        QString link = "http://openweathermap.org/city/" + locationCode;
        double pressure = 0.0;

        QJsonObject weatherData = jsonObject.find("weather").value().toArray().at(0).toObject();

        weatherCode = weatherData.find("id").value().toInt();
        temperature = nextBranch(jsonObject,"main").find("temp").value().toDouble();
        description = weatherData.find("description").value().toString();
        windSpeed = nextBranch(jsonObject,"wind").find("speed").value().toDouble();
        windDegree = nextBranch(jsonObject,"wind").find("deg").value().toInt();
        humidity = nextBranch(jsonObject,"main").find("humidity").value().toInt();
        sunriseTime = QDateTime::fromTime_t(nextBranch(jsonObject,"sys").find("sunrise").value().toInt());
        sunsetTime = QDateTime::fromTime_t(nextBranch(jsonObject,"sys").find("sunset").value().toInt());
        pressure = nextBranch(jsonObject,"main").find("pressure").value().toDouble();

        weatherPtr->setWeatherCode(weatherCode);
        weatherPtr->setWeatherDescription(description);
        weatherPtr->setTemperature(Util::calculateTemperature(temperature, temperatureUnit));
        weatherPtr->setHumidity(humidity);
        weatherPtr->setWindSpeed(Util::calculateWindSpeed(windSpeed, windSpeedUnit));
        weatherPtr->setWindDegree(windDegree);
        weatherPtr->setSunrise(sunriseTime.time().toString(Qt::SystemLocaleShortDate));
        weatherPtr->setSunset(sunsetTime.time().toString(Qt::SystemLocaleShortDate));
        weatherPtr->setPressure(Util::calculatePressure(pressure, pressureUnit));
        weatherPtr->setLocationLink(link);
        weatherPtr->setLocationId(locationId);
    }
    return weatherPtr;
}
