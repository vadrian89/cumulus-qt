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
#ifndef WEATHER_H
#define WEATHER_H

#include <QObject>
#include <QMap>
#include <QThread>
#include <QTime>

#include "DbConnection.h"
#include "Util.h"
#include "YWeatherController.h"
#include "OwmWeatherController.h"
#include "Forecast.h"
#include "WundWeatherController.h"
class WeatherType : public QObject {
    Q_OBJECT
    QString m_weather, m_weatherIcon, m_weatherDescription, m_location, m_locationLink, m_tempUnit,
    m_sunrise, m_sunset, m_speedUnit, m_weatherApi;
    float m_pressure;
    int m_weatherCode, m_temperature, m_windSpeed, m_windDegree, m_humidity;
    QMap<int, QString> *yahooIcons, *owmIcons, *wundIcons;
    QMap<QString, QString> *nightFonts;
    int m_tempMin, m_tempMax;
    bool m_loadFinished;
    QList<QObject*> m_forecastList;

    Q_PROPERTY(QString weather READ weather WRITE setWeather NOTIFY weatherChanged)
    Q_PROPERTY(int weatherCode READ weatherCode WRITE setWeatherCode NOTIFY weatherCodeChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon WRITE setWeatherIcon NOTIFY weatherIconChanged)
    Q_PROPERTY(QString weatherDescription READ weatherDescription WRITE setWeatherDescription NOTIFY weatherDescriptionChanged)
    Q_PROPERTY(int temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)
    Q_PROPERTY(double pressure READ pressure WRITE setPressure NOTIFY pressureChanged)
    Q_PROPERTY(QString tempUnit READ tempUnit WRITE setTempUnit NOTIFY tempUnitChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(QString locationLink READ locationLink WRITE setLocationLink NOTIFY locationLinkChanged)
    Q_PROPERTY(int windSpeed READ windSpeed WRITE setWindSpeed NOTIFY windSpeedChanged)
    Q_PROPERTY(int windDegree READ windDegree WRITE setWindDegree NOTIFY windDegreeChanged)
    Q_PROPERTY(int humidity READ humidity WRITE setHumidity NOTIFY humidityChanged)
    Q_PROPERTY(QString sunrise READ sunrise WRITE setSunrise NOTIFY sunriseChanged)
    Q_PROPERTY(QString sunset READ sunset WRITE setSunset NOTIFY sunsetChanged)
    Q_PROPERTY(int tempMax READ tempMax WRITE setTempMax NOTIFY tempMaxChanged)
    Q_PROPERTY(int tempMin READ tempMin WRITE setTempMin NOTIFY tempMinChanged)
    Q_PROPERTY(bool loadFinished READ loadFinished WRITE setLoadFinished NOTIFY loadFinishedChanged)
    Q_PROPERTY(QList<QObject*> forecastList READ forecastList WRITE setForecastList NOTIFY forecastListChanged)
    Q_PROPERTY(QString speedUnit READ speedUnit WRITE setSpeedUnit NOTIFY speedUnitChanged)
    Q_PROPERTY(QString weatherApi READ weatherApi WRITE setWeatherApi NOTIFY weatherApiChanged)

    int locationId();
    void loadData();
    void loadData(QSqlQuery &query);
    void mapYahooIcon();
    void mapOwmIcons();
    void mapNightIcons();
    void mapWundIcons();
    QString findIcon(int weatherCode);
    QString findNightIcon(const QString fontCode) const;
    QMap<QString,QString> searchCriteria();
    void updateForecastTemp(const QString &oldUnit);
    YWeatherController *yweather;
    OwmWeatherController *owmWeather;
    WundWeatherController *wundWeather;
    QThread workerThread;
    bool clearLocationCode();
public:
    explicit WeatherType(QObject *parent = 0);
    void setWeather(const QString &weather);
    QString weather() const;
    void setWeatherCode(const int &weatherCode);
    int weatherCode() const;
    void setWeatherIcon(const QString &weatherIcon);
    QString weatherIcon() const;
    void setWeatherDescription(const QString &weatherDescription);
    QString weatherDescription() const;
    void setTemperature(const int &temp);
    int temperature() const;
    void setPressure(const double &pressure);
    double pressure() const;
    void setHumidity(const float &humidity);
    float humidity() const;
    void setWindSpeed(const int &windspeed);
    int windSpeed() const;
    void setWindDegree(const int &windDegree);
    int windDegree() const;
    void setTempUnit(const QString &tempUnit);
    QString tempUnit() const;
    void setLocation(const QString &location);
    QString location() const;
    void setLocationLink(const QString &locationLink);
    QString locationLink() const;
    void setSunrise(const QString &sunrise);
    QString sunrise() const;
    void setSunset(const QString &sunset);
    QString sunset() const;    
    void setTempMax(const int &tempMax);
    int tempMax() const;
    void setTempMin(const int &tempMin);
    int tempMin() const;
    bool loadFinished() const;
    void setLoadFinished(const bool &loadFinished);
    QList<QObject*> forecastList() const;
    void setForecastList(const QList<QObject*> &list);
    QString speedUnit() const;
    void setSpeedUnit(const QString &unit);
    QString weatherApi() const;
    void setWeatherApi(const QString &weatherApi);
public slots:
    void getWeatherData();
    void setWeatherData();    
    QList<QObject*> getForecastData();
    void changeTempUnit(const QString &unit);
    void changeSpeedUnit(const QString &unit);
signals:
    void weatherChanged();
    void weatherCodeChanged();
    void weatherDescriptionChanged();
    void temperatureChanged();
    void tempUnitChanged();
    void locationChanged();
    void windSpeedChanged();
    void windDegreeChanged();
    void humidityChanged();
    void weatherDataChanged();
    void locationLinkChanged();
    void pressureChanged();
    void weatherIconChanged();
    void sunriseChanged();
    void sunsetChanged();
    void tempMinChanged();
    void tempMaxChanged();
    void noLocationSet();
    void dataDownloadFinished();
    void loadFinishedChanged();
    void forecastListChanged();
    void speedUnitChanged();
    void weatherApiChanged();
    void networkError(const QString &error);
};

#endif // WEATHER_H
