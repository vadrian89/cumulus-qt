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

#include <QDebug>
#include <memory>

using namespace std;

WeatherType::WeatherType(QObject *parent) : QObject(parent){}

void WeatherType::getWeatherData(){
    //TODO; add last update condition correctly
    SettingsController settings;
    DatabaseHelper dbHelper;
    if ((settings.currentLocationId() > 0 && dbHelper.lastLocationId() > 0) || settings.useGps()) {
        if (settings.weatherApi() == "y") {
            weatherController = new YWeatherController();
        }
        else if (settings.weatherApi() == "wund") {
            weatherController = new WundWeatherController();
        }
        else {
            weatherController = new OwmWeatherController();
        }
        thread = new QThread();
        weatherController->moveToThread(thread);
        connect(thread, SIGNAL(started()), weatherController, SLOT(getWeather()));
        connect(weatherController, SIGNAL(weatherSet(weather_struct)), this, SLOT(setWeatherData(weather_struct)));
        connect(weatherController, SIGNAL(networkError(QString)), this, SIGNAL(networkError(QString)));
        connect(this, SIGNAL(weatherChanged()), thread, SLOT(quit()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), weatherController, SLOT(deleteLater()));
        thread->start();
    }
}

void WeatherType::setWeatherData(const weather_struct &weather) {
    setSunrise(weather.m_sunrise);
    setSunset(weather.m_sunset);
    setWeatherCode(weather.m_weatherCode);
    setWeatherDescription(weather.m_weatherDescription);
    setTemperature(weather.m_temperature);
    setPressure(weather.m_pressure);
    setHumidity(weather.m_humidity);
    setWindSpeed(weather.m_windSpeed);
    setWindDegree(weather.m_windDegree);
    setTempMax(weather.m_tempMax);
    setTempMin(weather.m_tempMin);
    setLocationLink(weather.m_locationLink);
    setLocation(weather.m_location);
    m_forecastList.clear();
    SettingsController settings;
    for (Forecast *forecast : *weather.m_forecastListPtr) {
        Forecast *forec = new Forecast();
        forec->setWeatherIcon(Util::findFontCode(settings.weatherApi(), QString::number(forecast->weatherCode())));
        forec->setTempLow(forecast->tempLow());
        forec->setTempHigh(forecast->tempHigh());
        forec->setForecastDesc(forecast->forecastDesc());
        forec->setForecastDate(forecast->forecastDate());
        forec->setLocationId(forecast->locationId());
        m_forecastList.append(forec);
    }
    emit forecastListChanged();
    emit weatherDataChanged();
}

void WeatherType::setWeatherData() {
    SettingsController settings;
    DatabaseHelper dbHelper;
    const weather_struct weather = dbHelper.getWeather(settings.currentLocationId());
    setSunrise(weather.m_sunrise);
    setSunset(weather.m_sunset);
    setWeatherCode(weather.m_weatherCode);
    setWeatherDescription(weather.m_weatherDescription);
    setTemperature(weather.m_temperature);
    setPressure(weather.m_pressure);
    setHumidity(weather.m_humidity);
    setWindSpeed(weather.m_windSpeed);
    setWindDegree(weather.m_windDegree);
    setTempMax(weather.m_tempMax);
    setTempMin(weather.m_tempMin);
    setLocationLink(weather.m_locationLink);
    setLocation(weather.m_location);
    setForecastList(weather.m_forecastList);
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
        SettingsController settings;
        setWeatherIcon(Util::findFontCode(settings.weatherApi(), QString::number(m_weatherCode)));
        emit weatherCodeChanged();
    }
}

void WeatherType::setWeatherIcon(const QString &weatherIcon) {
    if (weatherIcon != m_weatherIcon) {
        m_weatherIcon = weatherIcon;
        if (QTime::currentTime() > QTime::fromString(m_sunset)) {
            QString nightFont = Util::findFontCode("night-font", m_weatherIcon);
            if (nightFont.trimmed().size() > 0)
                m_weatherIcon = nightFont;
        }
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
        m_location = m_location.remove(m_location.indexOf(","), m_location.size());
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
        if (QTime::currentTime() > QTime::fromString(m_sunset, "HH:mm")) {
            QString nightFont = Util::findFontCode("night-font", m_weatherIcon);
            if (nightFont.trimmed().size() > 0)
                setWeatherIcon(nightFont);
        }
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

QString WeatherType::tempUnit() const {
    return m_tempUnit;
}

void WeatherType::setTempUnit(const QString &tempUnit) {
    if (tempUnit != m_tempUnit) {
        if (m_tempUnit.trimmed().size() > 0) {
            updateTemperature(tempUnit);
        }
        m_tempUnit = tempUnit;
        emit tempUnitChanged();
    }
}

void WeatherType::updateTemperature(const QString &tempUnit) {
    setTemperature(Util::calculateTemperature(m_temperature, m_tempUnit, tempUnit));
    setTempMax(Util::calculateTemperature(m_tempMax, m_tempUnit, tempUnit));
    setTempMin(Util::calculateTemperature(m_tempMin, m_tempUnit, tempUnit));
    updateForecastTemp(tempUnit);
}

void WeatherType::updateForecastTemp(const QString &tempUnit) {
    for (int i = 0; i < m_forecastList.size(); i++) {
        Forecast *f = (Forecast*)m_forecastList.at(i);
        f->setTempHigh(Util::calculateTemperature(f->tempHigh(), m_tempUnit, tempUnit));
        f->setTempLow(Util::calculateTemperature(f->tempLow(), m_tempUnit, tempUnit));
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

QString WeatherType::speedUnit() const {
    return m_speedUnit;
}

void WeatherType::setSpeedUnit(const QString &speedUnit) {
    if (m_speedUnit != speedUnit) {
        if (m_speedUnit.trimmed().size() > 0)
            setWindSpeed(Util::calculateWindSpeed(m_windSpeed, m_speedUnit, speedUnit));
        m_speedUnit = speedUnit;
        emit speedUnitChanged();
    }
}
