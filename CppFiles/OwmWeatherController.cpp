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
    SettingsController settings;
    apiKey = settings.apiKey();
    temperatureUnit = "k";
    locationCode = "";
}

void OwmWeatherController::searchByGps(const double &lat, const double &lon) {
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getWeatherFromJson(QJsonObject)));
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toLower();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.openweathermap.org/data/2.5/weather?lat=" + QString::number(lat) + "&lon=" + QString::number(lon) + "&appid=" + apiKey + "&lang=" + systemLang);
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
    if (!jsonObject.isEmpty()) {
        locationCode = QString::number(jsonObject.find("id").value().toInt());
        m_weather.m_location = jsonObject.find("name").value().toString();
        m_weather.m_location.append(", ").append(nextBranch(jsonObject, "sys").find("country").value().toString());
        SettingsController settings;
        QString windSpeedUnit = "m/s";
        QString pressureUnit = "mbar";
        QJsonObject weatherData = jsonObject.find("weather").value().toArray().at(0).toObject();
        m_weather.m_weatherCode = weatherData.find("id").value().toInt();        
        m_weather.m_temperature = nextBranch(jsonObject,"main").find("temp").value().toDouble();
        m_weather.m_temperature = Util::calculateTemperature(m_weather.m_temperature, temperatureUnit, settings.tempUnit());
        m_weather.m_weatherDescription = weatherData.find("description").value().toString();
        m_weather.m_windSpeed = nextBranch(jsonObject,"wind").find("speed").value().toDouble();
        m_weather.m_windSpeed = Util::calculateWindSpeed(m_weather.m_windSpeed, windSpeedUnit, settings.windSpeedUnit());
        m_weather.m_windDegree = nextBranch(jsonObject,"wind").find("deg").value().toInt();
        m_weather.m_humidity = nextBranch(jsonObject,"main").find("humidity").value().toInt();
        QDateTime sunriseTime = QDateTime::fromTime_t(nextBranch(jsonObject,"sys").find("sunrise").value().toInt());
        m_weather.m_sunrise = sunriseTime.time().toString(Qt::SystemLocaleShortDate);
        QDateTime sunsetTime = QDateTime::fromTime_t(nextBranch(jsonObject,"sys").find("sunset").value().toInt());
        m_weather.m_sunset = sunsetTime.time().toString(Qt::SystemLocaleShortDate);
        m_weather.m_pressure = nextBranch(jsonObject,"main").find("pressure").value().toDouble();
        m_weather.m_pressure = Util::calculatePressure(m_weather.m_pressure, pressureUnit);
        m_weather.m_locationLink = "http://openweathermap.org/city/" + locationCode;
        m_weather.m_locationId = settings.currentLocationId();
        if (saveLocation(locationCode) && saveWeather(m_weather))
            getForecast(locationCode);
    }    
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
        Forecast *forecast = new Forecast(0);
        QJsonObject weatherInfo = forecastJson.toObject().find("weather").value().toArray().at(0).toObject();
        weatherCode = weatherInfo.find("id").value().toInt();
        tempHigh = nextBranch(forecastJson.toObject(), "temp").find("day").value().toDouble();
        tempLow = nextBranch(forecastJson.toObject(), "temp").find("night").value().toDouble();
        date = QDate::fromString(QDateTime::fromTime_t(forecastJson.toObject().find("dt").value().toInt()).toString("dd/MM/yyyy"), "dd/MM/yyyy");
        description = weatherInfo.find("description").value().toString();
        forecast->setWeatherCode(weatherCode);
        forecast->setTempLow(Util::calculateTemperature(tempLow, temperatureUnit, settings.tempUnit()));
        forecast->setTempHigh(Util::calculateTemperature(tempHigh, temperatureUnit, settings.tempUnit()));
        forecast->setForecastDesc(description);
        forecast->setForecastDate(date.toString("dd/MMM/yyyy"));
        forecast->setLocationId(settings.currentLocationId());
        forecastList.append(forecast);
    }
    if (!forecastList.isEmpty()) {
        m_weather.m_tempMax = forecastList.at(1)->tempHigh();
        m_weather.m_tempMin = forecastList.at(1)->tempLow();
        saveForecast(forecastList);
    }
}
