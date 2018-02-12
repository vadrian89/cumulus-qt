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
#include "WeatherType.h"
#include "Forecast.h"
#include "Util.h"
#include "DatabaseHelper.h"
#include "Weather.h"

#include <QDebug>
#include <memory>

using namespace std;

WeatherType::WeatherType(QObject *parent) : QObject(parent){}

void WeatherType::getWeatherData(){
    qDebug() << "WeatherType::getWeatherData >> Weather data request.";
    unique_ptr<DatabaseHelper> dbHelperPtr(new DatabaseHelper);
    if (dbHelperPtr) {
        unique_ptr<Location> locationPtr(dbHelperPtr.get()->getLocation(1));
        if (locationPtr) {
            int locationCodeSize = locationPtr.get()->m_locationCode.trimmed().size();
            if (Util::getWeatherApi() == "y") {
                yweather = new YWeatherController();
                connect(yweather, SIGNAL(forecastChanged()), this, SLOT(setWeatherData()));
                connect(yweather, SIGNAL(networkError(QString)), this, SLOT(setWeatherData()));
                connect(yweather, SIGNAL(networkError(QString)), this, SIGNAL(networkError(QString)));
                if (locationCodeSize > 0)
                    yweather->searchBycode(locationPtr.get()->m_locationCode);
                else
                    yweather->searchByLocation(locationPtr.get()->m_locationName);
            }
            else if (Util::getWeatherApi() == "wund") {
                wundWeather = new WundWeatherController();
                connect(wundWeather, SIGNAL(forecastChanged()), this, SLOT(setWeatherData()));
                connect(wundWeather, SIGNAL(dataDownloaded()), this, SIGNAL(dataDownloadFinished()));
                connect(wundWeather, SIGNAL(networkError(QString)), this, SLOT(setWeatherData()));
                connect(wundWeather, SIGNAL(networkError(QString)), this, SIGNAL(networkError(QString)));
                if (locationCodeSize > 0)
                    wundWeather->searchBycode(locationPtr.get()->m_locationCode);
                else
                    wundWeather->searchByLocation(locationPtr.get()->m_locationName);
            }
            else {
                owmWeather = new OwmWeatherController();
                connect(owmWeather, SIGNAL(forecastChanged()), this, SLOT(setWeatherData()));
                connect(owmWeather, SIGNAL(dataDownloaded()), this, SIGNAL(dataDownloadFinished()));
                connect(owmWeather, SIGNAL(networkError(QString)), this, SLOT(setWeatherData()));
                connect(owmWeather, SIGNAL(networkError(QString)), this, SIGNAL(networkError(QString)));
                if (locationCodeSize > 0)
                    owmWeather->searchBycode(locationPtr.get()->m_locationCode);
                else
                    owmWeather->searchByLocation(locationPtr.get()->m_locationName);
            }
        }
        else {
            emit noLocationSet();
        }
    }
    else {
        emit noLocationSet();
    }
}

void WeatherType::setWeatherData() {
    unique_ptr<DatabaseHelper> dbHelperPtr(new DatabaseHelper);
    if (dbHelperPtr) {
        unique_ptr<Weather> weatherPtr(dbHelperPtr.get()->getWeather(1));
        setLocation("Undefined");
        if (weatherPtr) {
            setWeatherCode(weatherPtr->weatherCode());
            setWeatherIcon(weatherPtr->weatherIcon());
            setWeatherDescription(weatherPtr->weatherDescription());
            setTemperature(weatherPtr->temperature());
            setPressure(weatherPtr->pressure());
            setHumidity(weatherPtr->humidity());
            setWindSpeed(weatherPtr->windSpeed());
            setWindDegree(weatherPtr->windDegree());
            setSunrise(weatherPtr->sunrise());
            setSunset(weatherPtr->sunset());
            setTempMax(weatherPtr->tempMax());
            setTempMin(weatherPtr->tempMin());
            setTempUnit(weatherPtr->tempUnit());
            setSpeedUnit(weatherPtr->speedUnit());
            setLocationLink(weatherPtr->locationLink());
            setLocation(weatherPtr->location());
            setForecastList(weatherPtr->forecastList());
        }
    }
    emit weatherDataChanged();
}

void WeatherType::setWeather(const QString &weather) {
    if (weather != m_weather) {
        m_weather = weather;
        emit weatherChanged();
    }
}

void WeatherType::setWeatherCode(const int &weatherCode) {
    if (weatherCode != m_weatherCode) {
        m_weatherCode = weatherCode;
        emit weatherCodeChanged();
    }
}

void WeatherType::setWeatherIcon(const QString &weatherIcon) {
    if (weatherIcon != m_weatherIcon) {
        m_weatherIcon = weatherIcon;
        emit weatherIconChanged();
    }
}

QString WeatherType::weatherIcon() const {
    return m_weatherIcon;
}

void WeatherType::setWeatherDescription(const QString &weatherDescription) {
    if (weatherDescription != m_weatherDescription) {
        m_weatherDescription = Util::firstLetterUp(weatherDescription);
        emit weatherDescriptionChanged();
    }
}

QString WeatherType::weatherDescription() const {
    return m_weatherDescription;
}

void WeatherType::setTemperature(const int &temp) {
    if (temp != m_temperature) {
        m_temperature = temp;
        emit temperatureChanged();
    }
}

int WeatherType::temperature() const {
    return m_temperature;
}

void WeatherType::setPressure(const double &pressure) {
    if (m_pressure != pressure) {
        m_pressure = QString::number(pressure, 'd', 2).toDouble();
        emit pressureChanged();
    }
}

double WeatherType::pressure() const {
    return m_pressure;
}

void WeatherType::setHumidity(const float &humidity) {
    if (humidity != m_humidity) {
        m_humidity = humidity;
        emit humidityChanged();
    }
}

float WeatherType::humidity() const {
    return m_humidity;
}

void WeatherType::setWindSpeed(const int &windSpeed) {
    if (windSpeed!= m_windSpeed) {
        m_windSpeed = windSpeed;
        emit windSpeedChanged();
    }
}

int WeatherType::windSpeed() const {
    return m_windSpeed;
}

void WeatherType::setWindDegree(const int &windDegree) {
    if (windDegree != m_windDegree) {
        m_windDegree = windDegree;
        emit windDegreeChanged();
    }
}

int WeatherType::windDegree() const {
    return m_windDegree;
}

void WeatherType::setLocation(const QString &location) {
    if (location != m_location) {
        m_location = location;
        emit locationChanged();
    }
}

void WeatherType::setLocationLink(const QString &locationLink) {
    if (locationLink != m_locationLink) {
        m_locationLink = locationLink;
        emit locationLinkChanged();
    }
}

QString WeatherType::weather() const {
    return m_weather;
}

int WeatherType::weatherCode() const {
    return m_weatherCode;
}

QString WeatherType::location() const {
    return m_location;
}

QString WeatherType::locationLink() const {
    return m_locationLink;
}

void WeatherType::setSunrise(const QString &sunrise) {
    if (m_sunrise != sunrise) {
        m_sunrise = sunrise;
        emit sunriseChanged();
    }
}

QString WeatherType::sunrise() const {
    return m_sunrise;
}

void WeatherType::setSunset(const QString &sunset) {
    if (m_sunset != sunset) {
        m_sunset = sunset;
        emit sunsetChanged();
    }
}

QString WeatherType::sunset() const {
    return m_sunset;
}

void WeatherType::setTempMax(const int &tempMax) {
    if (m_tempMax != tempMax) {
        m_tempMax = tempMax;
        emit tempMaxChanged();
    }
}

int WeatherType::tempMax() const {
    return m_tempMax;
}

void WeatherType::setTempMin(const int &tempMin) {
    if (m_tempMin != tempMin) {
        m_tempMin = tempMin;
        emit tempMinChanged();
    }
}

int WeatherType::tempMin() const {
    return m_tempMin;
}

void WeatherType::setLoadFinished(const bool &loadFinished) {
    if (m_loadFinished != loadFinished) {
        m_loadFinished = loadFinished;
        if (m_loadFinished == true) {
            if(Util::getWeatherApi() == "y") {
                yweather->saveDataToDb();
            }
            else if(Util::getWeatherApi() == "wund") {
                wundWeather->saveDataToDb();
            }
            else {
                owmWeather->saveDataToDb();
            }

        }
        emit loadFinishedChanged();
    }
}

bool WeatherType::loadFinished() const {
    return m_loadFinished;
}

QString WeatherType::tempUnit() const {
    return m_tempUnit;
}

void WeatherType::setTempUnit(const QString &tempUnit) {
    if (tempUnit != m_tempUnit) {
        m_tempUnit = tempUnit;
        emit tempUnitChanged();
    }
}

void WeatherType::changeTempUnit(const QString &unit) {
    if (m_tempUnit != unit) {
        QString oldUnit = Util::getTemperatureUnit().toLower();
        Util::setTemperatureUnit(unit);
        setTemperature(Util::calculateTemperature(m_temperature, oldUnit));
        setTempMax(Util::calculateTemperature(m_tempMax, oldUnit));
        setTempMin(Util::calculateTemperature(m_tempMin, oldUnit));
        updateForecastTemp(oldUnit);
        setTempUnit(Util::temperatureUnitSymbol());
    }
}

void WeatherType::updateForecastTemp(const QString &oldUnit) {
    for (int i = 0; i < m_forecastList.size(); i++) {
        Forecast *f = (Forecast*)m_forecastList.at(i);
        f->setTempHigh(Util::calculateTemperature(f->tempHigh(), oldUnit));
        f->setTempLow(Util::calculateTemperature(f->tempLow(), oldUnit));
    }
    emit forecastListChanged();
}

QList<QObject*> WeatherType::forecastList() const {
    return m_forecastList;
}

void WeatherType::setForecastList(const QList<QObject*> &list) {
    if (m_forecastList != list) {
        m_forecastList = list;
        emit forecastListChanged();
    }
}

void WeatherType::changeSpeedUnit(const QString &unit) {
    if (m_speedUnit != unit) {
        QString oldUnit = Util::getWindSpeedUnit().toLower();
        Util::setWindSpeedUnit(unit);
        setWindSpeed(Util::calculateWindSpeed(m_windSpeed, oldUnit));
        setSpeedUnit(Util::speedUnitSymbol());
    }
}

QString WeatherType::speedUnit() const {
    return m_speedUnit;
}

void WeatherType::setSpeedUnit(const QString &unit) {
    if (m_speedUnit != unit) {
        m_speedUnit = unit;
        emit speedUnitChanged();
    }
}

bool WeatherType::clearLocationCode() {
    unique_ptr<DatabaseHelper> dbHelperPtr(new DatabaseHelper);
    bool result = false;
    if (dbHelperPtr) {
        result = dbHelperPtr.get()->clearLocationCode(1);
    }
    return result;
}

QString WeatherType::weatherApi() const {
    return m_weatherApi;
}

void WeatherType::setWeatherApi(const QString &weatherApi) {
    if (m_weatherApi != weatherApi && clearLocationCode()) {
        m_weatherApi = weatherApi;
        Util::setWeatherApi(weatherApi);
        emit weatherApiChanged();
    }
}
