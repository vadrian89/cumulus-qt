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
    temperatureUnit = "k";
    locationCode = "";
}

void OwmWeatherController::searchByLocation(QString &location) {
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getLocationFromJson(QJsonObject)));
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toLower();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.openweathermap.org/data/2.5/weather?q=" + location + "&appid=" + apiKey + "&lang=" + systemLang);
}

void OwmWeatherController::getLocationFromJson(const QJsonObject &jsonObject) {
    disconnect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getLocationFromJson(QJsonObject)));
    locationCode = QString::number(jsonObject.find("id").value().toInt());
    if (saveLocation(locationCode))
        getWeatherFromJson(jsonObject);
}

void OwmWeatherController::searchBycode(QString &code) {
    locationCode = code;
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getWeatherFromJson(QJsonObject)));
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toLower();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.openweathermap.org/data/2.5/weather?id=" + code + "&appid=" + apiKey + "&lang=" + systemLang);
}

void OwmWeatherController::getForecast(const QString &code) {
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getForecastFromJson(QJsonObject)));
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toLower();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.openweathermap.org/data/2.5/forecast/daily?id=" + code + "&appid=" + apiKey + "&lang=" + systemLang);
}

void OwmWeatherController::getWeatherFromJson(const QJsonObject &jsonObject) {
    disconnect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getWeatherFromJson(QJsonObject)));
    Weather *weatherPtr = nullptr;
    SettingsController settings;
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
        weatherPtr->setLocationId(settings.currentLocationId());
    }
    if (saveWeather(weatherPtr))
        getForecast(locationCode);
}

void OwmWeatherController::getForecastFromJson(const QJsonObject &jsonObject) {
    QJsonArray forecastArray = jsonObject.find("list").value().toArray();
    SettingsController settings;
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
        forecast->setLocationId(settings.currentLocationId());
        forecastList.append(forecast);
    }
    saveForecast(forecastList);
}
