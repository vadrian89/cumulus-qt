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
    SettingsController settings;
    apiKey = settings.apiKey();
    temperatureUnit = "c";
    locationCode = "";
    name = "wund";
}

void WundWeatherController::searchByGps(const double &lat, const double &lon) {
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getWeatherFromJson(QJsonObject)));
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toUpper();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.wunderground.com/api/" + apiKey + "/conditions/lang:" + systemLang + "/q/" + QString::number(lat) + "," + QString::number(lon) + ".json");
}

void WundWeatherController::searchByLocation(QString &location) {
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getLocationFromJson(QJsonObject)));
    dataController->getDataFromUrl("http://autocomplete.wunderground.com/aq?query=" + location);
}

void WundWeatherController::getLocationFromJson(const QJsonObject &jsonObject) {
    disconnect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getLocationFromJson(QJsonObject)));
    QJsonArray array = jsonObject.find("RESULTS").value().toArray();
    locationCode = array.at(0).toObject().find("zmw").value().toString();
    searchBycode(locationCode);
}

void WundWeatherController::searchBycode(QString &code) {
    locationCode = code;
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getWeatherFromJson(QJsonObject)));
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toUpper();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.wunderground.com/api/" + apiKey + "/conditions/lang:" + systemLang + "/q/zmw:" + code + ".json");
}

void WundWeatherController::getWeatherFromJson(const QJsonObject &jsonObject) {
    disconnect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getWeatherFromJson(QJsonObject)));
    if (!jsonObject.isEmpty()) {
        SettingsController settings;
        QString windSpeedUnit = "kph";
        QString pressureUnit = "mbar";
        QJsonObject weatherData = nextBranch(jsonObject, "current_observation");
        m_weather.m_location = nextBranch(weatherData, "display_location").find("full").value().toString();
        if (locationCode.isEmpty()) {
            locationCode = nextBranch(weatherData, "display_location").find("zip").value().toString()
                    .append(".")
                    .append(nextBranch(weatherData, "display_location").find("magic").value().toString())
                    .append(".")
                    .append(nextBranch(weatherData, "display_location").find("wmo").value().toString());
        }        
        m_weather.m_weatherCode = Util::findFontCode("wund-map", weatherData.find("icon").value().toString()).toInt();
        m_weather.m_temperature = weatherData.find("temp_c").value().toDouble();
        m_weather.m_temperature = Util::calculateTemperature(m_weather.m_temperature, temperatureUnit, settings.tempUnit());
        m_weather.m_weatherDescription = weatherData.find("weather").value().toString();
        m_weather.m_windSpeed = weatherData.find("wind_kph").value().toDouble();
        m_weather.m_windSpeed = Util::calculateWindSpeed(m_weather.m_windSpeed, windSpeedUnit, settings.windSpeedUnit());
        m_weather.m_windDegree = weatherData.find("wind_degrees").value().toInt();
        m_weather.m_humidity = weatherData.find("relative_humidity").value().toString().remove("%").toInt();
        m_weather.m_pressure = weatherData.find("pressure_mb").value().toString().toDouble();
        m_weather.m_pressure = Util::calculatePressure(m_weather.m_pressure, pressureUnit);
        m_weather.m_locationLink = weatherData.find("ob_url").value().toString();
        m_weather.m_locationId = settings.currentLocationId();
        getAstronomy(locationCode);
    }    
}

void WundWeatherController::getAstronomy(const QString &code) {
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getAstronomyFromJson(QJsonObject)));
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toUpper();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.wunderground.com/api/" + apiKey + "/astronomy/lang:" + systemLang + "/q/zmw:" + code + ".json");
}

void WundWeatherController::getAstronomyFromJson(const QJsonObject &jsonObject) {
    disconnect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getAstronomyFromJson(QJsonObject)));
    QJsonObject sunriseData = nextBranch(nextBranch(jsonObject, "sun_phase"), "sunrise");
    QJsonObject sunsetData = nextBranch(nextBranch(jsonObject, "sun_phase"), "sunset");
    QDateTime sunrise = QDateTime::fromString(sunriseData.find("hour").value().toString() + ":" +
            sunriseData.find("minute").value().toString(), "h:mm");
    QDateTime sunset = QDateTime::fromString(sunsetData.find("hour").value().toString() + ":" +
            sunsetData.find("minute").value().toString(), "h:mm");
    m_weather.m_sunrise = sunrise.time().toString(Qt::SystemLocaleShortDate);
    m_weather.m_sunset = sunset.time().toString(Qt::SystemLocaleShortDate);
    if (saveLocation(locationCode) && saveWeather(m_weather))
        getForecast(locationCode);
}

void WundWeatherController::getForecast(const QString &code) {
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getForecastFromJson(QJsonObject)));
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toUpper();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.wunderground.com/api/" + apiKey + "/forecast10day/lang:" + systemLang + "/q/zmw:" + code + ".json");
}

void WundWeatherController::getForecastFromJson(const QJsonObject &jsonObject) {
    QJsonArray forecastArray = nextBranch(nextBranch(jsonObject, "forecast"), "simpleforecast").find("forecastday").value().toArray();
    SettingsController settings;
    int weatherCode = -1;
    int tempHigh = 0;
    int tempLow = 0;
    QDate date = QDate::currentDate();
    QString description = "";
    QList<Forecast*> forecastList;
    for (QJsonValue forecastJson : forecastArray) {
        Forecast *forecast = new Forecast();
        weatherCode = Util::findFontCode("wund-map", forecastJson.toObject().find("icon").value().toString()).toInt();
        tempHigh = nextBranch(forecastJson.toObject(), "high").find("celsius").value().toString().toInt();
        tempLow = nextBranch(forecastJson.toObject(), "low").find("celsius").value().toString().toInt();
        date = dateFromJson(nextBranch(forecastJson.toObject(), "date"));
        description = forecastJson.toObject().find("conditions").value().toString();
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

QDate WundWeatherController::dateFromJson(const QJsonObject &jsonObject) {
    QDate date(jsonObject.find("year").value().toInt(),
               jsonObject.find("month").value().toInt(),
               jsonObject.find("day").value().toInt());
    return date;
}
