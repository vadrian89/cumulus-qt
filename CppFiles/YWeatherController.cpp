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

YWeatherController::YWeatherController(QObject *parent) : AbstractWeatherController(parent) {
    temperatureUnit = "f";
    locationCode = "";
}

void YWeatherController::searchByGps(const double &lat, const double &lon) {
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getLocationFromJson(QJsonObject)));
    dataController->getDataFromUrl("https://query.yahooapis.com/v1/public/yql?q=select * from weather.forecast where woeid in (select woeid from geo.places(1) where text=\"(" + QString::number(lat) + "," + QString::number(lon) + ")\")&format=json");
}

void YWeatherController::searchByLocation(QString &location) {
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getLocationFromJson(QJsonObject)));
    dataController->getDataFromUrl("https://query.yahooapis.com/v1/public/yql?q=select * from weather.forecast where woeid in (select woeid from geo.places(1) where text='" + location + "')&format=json");
}

void YWeatherController::getLocationFromJson(const QJsonObject &jsonObject) {
    disconnect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getLocationFromJson(QJsonObject)));
    QJsonObject resultsObject = nextBranch(jsonObject, "query").find("results").value().toObject();
    locationCode = nextBranch(resultsObject, "channel").find("link").value().toString();
    QRegExp reg("city-[0-9]+/");
    locationCode.remove(0, locationCode.indexOf(reg)).remove("city-").remove("/");
    getWeatherFromJson(jsonObject);
}

void YWeatherController::searchBycode(QString &code) {
    locationCode = code;
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getWeatherFromJson(QJsonObject)));
    dataController->getDataFromUrl("https://query.yahooapis.com/v1/public/yql?q=select * from weather.forecast where woeid=" + code +"&format=json");
}

void YWeatherController::getWeatherFromJson(const QJsonObject &jsonObject) {
    disconnect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getWeatherFromJson(QJsonObject)));
    if (!jsonObject.isEmpty()) {
        SettingsController settings;
        QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
        QString windSpeedUnit = "mph";
        QString pressureUnit = "mbar";
        QJsonObject channel = nextBranch(nextBranch(nextBranch(jsonObject, "query"), "results"), "channel");
        m_weather.m_location = nextBranch(channel, "location").find("city").value().toString().append(", ");
        m_weather.m_location.append(nextBranch(channel, "location").find("country").value().toString());
        QJsonObject units = nextBranch(channel, "units");
        windSpeedUnit = units.find("speed").value().toString().toLower();
        temperatureUnit = units.find("temperature").value().toString().toLower();
        QJsonObject item = nextBranch(channel, "item");
        QJsonObject wind = nextBranch(channel, "wind");
        QJsonObject atmosphere = nextBranch(channel, "atmosphere");
        QJsonObject astronomy = nextBranch(channel, "astronomy");
        m_weather.m_weatherCode = nextBranch(item, "condition").find("code").value().toString().toInt();
        m_weather.m_temperature = nextBranch(item, "condition").find("temp").value().toString().toInt();
        m_weather.m_temperature = Util::calculateTemperature(m_weather.m_temperature, temperatureUnit, settings.tempUnit());
        m_weather.m_weatherDescription = nextBranch(item, "condition").find("text").value().toString();
        m_weather.m_locationLink = channel.find("link").value().toString().remove(QRegExp("^http.+\\*"));
        m_weather.m_windSpeed = wind.find("speed").value().toString().toInt();
        m_weather.m_windSpeed = Util::calculateWindSpeed(m_weather.m_windSpeed, windSpeedUnit, settings.windSpeedUnit());
        m_weather.m_windDegree = wind.find("direction").value().toString().toInt();
        m_weather.m_humidity = atmosphere.find("humidity").value().toString().toInt();
        m_weather.m_sunrise = QLocale().toDateTime(astronomy.find("sunrise").value().toString(), "h:m ap").time().toString(Qt::SystemLocaleShortDate);
        m_weather.m_sunset = QLocale().toDateTime(astronomy.find("sunset").value().toString(), "h:m ap").time().toString(Qt::SystemLocaleShortDate);
        m_weather.m_pressure = atmosphere.find("pressure").value().toString().toDouble();
        m_weather.m_pressure = Util::calculatePressure(m_weather.m_pressure, pressureUnit);
        m_weather.m_locationId = settings.currentLocationId();
        QJsonObject resultsObject = nextBranch(jsonObject, "query").find("results").value().toObject();
        if (saveLocation(locationCode) && saveWeather(m_weather))
            getForecastFromJson(nextBranch(nextBranch(resultsObject, "channel"), "item"));
    }    
}

void YWeatherController::getForecastFromJson(const QJsonObject &jsonObject) {
    QJsonArray forecastArray = jsonObject.find("forecast").value().toArray();
    int weatherCode = -1;
    int tempHigh = 0;
    int tempLow = 0;
    QLocale yahooLocale(QLocale(QLocale::English));
    QDate date = QDate::currentDate();
    QString description = "";
    QList<Forecast*> forecastList;
    SettingsController settings;
    for (QJsonValue forecastJson : forecastArray) {
        Forecast *forecast = new Forecast();
        weatherCode = forecastJson.toObject().find("code").value().toString().toInt();
        tempHigh = forecastJson.toObject().find("high").value().toString().toInt();
        tempLow = forecastJson.toObject().find("low").value().toString().toInt();
        QString forecastDateStr = forecastJson.toObject().find("date").value().toString().replace(" ", "/");
        date = yahooLocale.toDate(forecastDateStr, "dd/MMM/yyyy");
        description = forecastJson.toObject().find("text").value().toString();
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
