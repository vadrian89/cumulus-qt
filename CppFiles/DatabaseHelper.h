/*
* Copyright (C) 2018 Adrian Verban <verbanady@gmail.com>
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
#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QObject>
#include <QSqlDatabase>
#include <QApplication>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QList>
#include <QSqlDriver>
#include <QTime>
#include <memory>

#include "Forecast.h"
#include "SettingsController.h"

using namespace std;

struct location_table_definition {
    const QString
    TABLE_NAME = "db_ma_location",
    LOC_ID_COLUMN = "loc_id",
    LOC_CODE_COLUMN = "loc_code",
    LOC_NAME_COLUMN = "loc_name";
};

struct location_struct {
    int m_locationId;
    QString m_locationCode, m_locationName;
};

struct weather_struct {
    QString m_weatherIcon, m_weatherDescription, m_location, m_locationLink, m_tempUnit,
    m_sunrise, m_sunset, m_speedUnit;
    float m_pressure;
    int m_weatherCode, m_temperature, m_windSpeed, m_windDegree, m_humidity, m_locationId,
    m_tempMin, m_tempMax;
    QList<QObject*> m_forecastList;
    QList<Forecast*> *m_forecastListPtr;
};

struct weather_table_definition {
    const QString
    TABLE_NAME = "db_tr_weather",
    WEATHER_ID_COLUMN = "weather_id",
    WEATHER_CODE_COLUMN = "weather_code",
    WEATHER_DESCRIPTION_COLUMN = "weather_description",
    WEATHER_TEMPERATURE_COLUMN = "weather_temperature",
    WEATHER_PRESSURE_COLUMN = "weather_pressure",
    WEATHER_HUMIDITY_COLUMN = "weather_humidity",
    WEATHER_WIND_SPEED_COLUMN = "weather_wind_speed",
    WEATHER_WIND_DEGREE_COLUMN = "weather_wind_degree",
    WEATHER_SUNRISE_COLUMN = "weather_sunrise",
    WEATHER_SUNSET_COLUMN = "weather_sunset",
    WEATHER_LINK_COLUMN = "weather_link",
    WEATHER_LOC_ID_COLUMN = "weather_loc_id";
};

struct forecast_table_definition {
    const QString
    TABLE_NAME = "db_tr_forecast",
    FOREC_ID_COLUMN = "forec_id",
    FOREC_DATE_COLUMN = "forec_date",
    FOREC_WEATHER_CODE_COLUMN = "forec_weather_code",
    FOREC_WEATHER_DESCRIPTION_COLUMN = "forec_weather_description",
    FOREC_TEMP_MIN_COLUMN = "forec_temp_min",
    FOREC_TEMP_MAX_COLUMN = "forec_temp_max",
    FOREC_PRESSURE_COLUMN = "forec_pressure",
    FOREC_HUMIDITY_COLUMN = "forec_humidity",
    FOREC_WIND_SPEED_COLUMN = "forec_wind_speed",
    FOREC_WIND_DEGREE_COLUMN = "forec_wind_degree",
    FOREC_LOC_ID_COLUMN = "forec_loc_id";
};

class DatabaseHelper : public QObject {
    Q_OBJECT
    QString locationTableCreationQuery();
    QString weatherTableCreationQuery();
    QString forecastTableCreationQuery();
    bool startCon(QSqlDatabase &db);
    bool stopCon(QSqlDatabase &db);
    QSqlDatabase getDatabase();
    QString databaseName(const QString &appName);
    bool databaseInit();
    void emitQueryError(const QString &method, const QString &errorString);
public:    
    static struct location_table_definition DB_MA_LOCATION;
    static struct weather_table_definition DB_TR_WEATHER;
    static struct forecast_table_definition DB_TR_FORECAST;
    explicit DatabaseHelper(QObject *parent = nullptr);
    bool clearLocationCode();
    location_struct getLocation(const int &locationId);
    location_struct findLocation(const QString &locationName);
    QList<location_struct> getLocationList();
    bool insertLocation(const location_struct &location);
    bool updateLocation(const location_struct &location);
    bool deleteLocation(const int &locationId);
    int lastLocationId();
    weather_struct getWeather(const int &locationId);
    bool insertWeather(const weather_struct &weatherPtr);
    bool deleteWeather(const int &locationId);
    QList<QObject*> getForecast(const int &locationId);
    bool insertForecast(const QList<Forecast*> &forecastList);
    bool insertForecast(const Forecast *forecast, const QSqlDatabase &db);
    bool deleteForecast(const int &locationId);
signals:
    void queryError(QString errorString);
    void querySuccessful();
};

#endif // DATABASEHELPER_H
