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
    temperatureUnit = "c";
    locationCode = "";
}

void WundWeatherController::searchByLocation(QString &location) {
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getLocationFromJson(QJsonObject)));
    dataController->getDataFromUrl("http://autocomplete.wunderground.com/aq?query=" + location);
}

void WundWeatherController::getLocationFromJson(const QJsonObject &jsonObject) {
    QJsonArray array = jsonObject.find("RESULTS").value().toArray();
    locationCode = array.at(0).toObject().find("zmw").value().toString();
    if (saveLocation(locationCode))
        searchBycode(locationCode);
}

void WundWeatherController::searchBycode(QString &code) {
    locationCode = code;
    disconnect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getLocationFromJson(QJsonObject)));
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getAstronomyFromJson(QJsonObject)));
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toUpper();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.wunderground.com/api/" + apiKey + "/astronomy/lang:" + systemLang + "/q/zmw:" + code + ".json");

}

void WundWeatherController::getAstronomyFromJson(const QJsonObject &jsonObject) {
    QJsonObject sunriseData = nextBranch(nextBranch(jsonObject, "sun_phase"), "sunrise");
    QJsonObject sunsetData = nextBranch(nextBranch(jsonObject, "sun_phase"), "sunset");
    sunrise = QDateTime::fromString(sunriseData.find("hour").value().toString() + ":" +
            sunriseData.find("minute").value().toString(), "h:mm");
    sunset = QDateTime::fromString(sunsetData.find("hour").value().toString() + ":" +
            sunsetData.find("minute").value().toString(), "h:mm");
    getAfterAstronomy(locationCode);
}

void WundWeatherController::getAfterAstronomy(const QString &code) {
    disconnect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getAstronomyFromJson(QJsonObject)));
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getWeatherFromJson(QJsonObject)));
    QString systemLang = QLocale::languageToString(QLocale::system().language()).toUpper();
    systemLang.resize(2);
    dataController->getDataFromUrl("http://api.wunderground.com/api/" + apiKey + "/conditions/lang:" + systemLang + "/q/zmw:" + code + ".json");
}

void WundWeatherController::getWeatherFromJson(const QJsonObject &jsonObject) {
    SettingsController settings;
    Weather *weatherPtr = nullptr;
    if (!jsonObject.isEmpty()) {
        weatherPtr = new Weather;
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

        weatherCode = Util::findFontCode("wund-map", weatherData.find("icon").value().toString()).toInt();
        temperature = weatherData.find("temp_c").value().toDouble();
        description = weatherData.find("weather").value().toString();
        windSpeed = weatherData.find("wind_kph").value().toDouble();
        windDegree = weatherData.find("wind_degrees").value().toInt();
        humidity = weatherData.find("relative_humidity").value().toString().remove("%").toInt();
        pressure = weatherData.find("pressure_mb").value().toString().toDouble();

        weatherPtr->setWeatherCode(weatherCode);
        weatherPtr->setWeatherDescription(description);
        weatherPtr->setTemperature(Util::calculateTemperature(temperature, temperatureUnit));
        weatherPtr->setHumidity(humidity);
        weatherPtr->setWindSpeed(Util::calculateWindSpeed(windSpeed, windSpeedUnit));
        weatherPtr->setWindDegree(windDegree);
        weatherPtr->setSunrise(sunrise.time().toString(Qt::SystemLocaleShortDate));
        weatherPtr->setSunset(sunset.time().toString(Qt::SystemLocaleShortDate));
        weatherPtr->setPressure(Util::calculatePressure(pressure, pressureUnit));
        weatherPtr->setLocationLink(link);
        weatherPtr->setLocationId(settings.currentLocationId());
    }
    if (saveWeather(weatherPtr))
        getForecast(locationCode);
}

void WundWeatherController::getForecast(const QString &code) {
    disconnect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getWeatherFromJson(QJsonObject)));
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
        forecast->setTempLow(Util::calculateTemperature(tempLow, temperatureUnit));
        forecast->setTempHigh(Util::calculateTemperature(tempHigh, temperatureUnit));
        forecast->setForecastDesc(description);
        forecast->setForecastDate(date.toString("dd/MMM/yyyy"));
        forecast->setLocationId(settings.currentLocationId());
        forecastList.append(forecast);
    }
    saveForecast(forecastList);
}

QDate WundWeatherController::dateFromJson(const QJsonObject &jsonObject) {
    QDate date(jsonObject.find("year").value().toInt(),
               jsonObject.find("month").value().toInt(),
               jsonObject.find("day").value().toInt());
    return date;
}
