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

void YWeatherController::searchByLocation(QString &location) {
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getLocationFromJson(QJsonObject)));
    dataController->getDataFromUrl("https://query.yahooapis.com/v1/public/yql?q=select woeid from geo.places(1) where text='" + location + "'&format=json");
}

void YWeatherController::getLocationFromJson(const QJsonObject &jsonObject) {
    QJsonObject resultsObject = nextBranch(jsonObject, "query").find("results").value().toObject();
    locationCode = nextBranch(resultsObject, "place").find("woeid").value().toString();
    if (saveLocation(locationCode))
        searchBycode(locationCode);
}

void YWeatherController::searchBycode(QString &code) {
    locationCode = code;
    disconnect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getLocationFromJson(QJsonObject)));
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(getWeatherFromJson(QJsonObject)));
    dataController->getDataFromUrl("https://query.yahooapis.com/v1/public/yql?q=select * from weather.forecast where woeid=" + code +"&format=json");
}

void YWeatherController::getWeatherFromJson(const QJsonObject &jsonObject) {
    Weather *weatherPtr = nullptr;
    SettingsController settings;
    if (!jsonObject.isEmpty()) {
        weatherPtr = new Weather;
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
    QJsonObject resultsObject = nextBranch(jsonObject, "query").find("results").value().toObject();
    if (saveWeather(weatherPtr))
        getForecastFromJson(nextBranch(nextBranch(resultsObject, "channel"), "item"));
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
        forecast->setTempLow(Util::calculateTemperature(tempLow, temperatureUnit));
        forecast->setTempHigh(Util::calculateTemperature(tempHigh, temperatureUnit));
        forecast->setForecastDesc(description);
        forecast->setForecastDate(date.toString("dd/MMM/yyyy"));
        forecast->setLocationId(settings.currentLocationId());
        forecastList.append(forecast);
    }
    saveForecast(forecastList);
}
