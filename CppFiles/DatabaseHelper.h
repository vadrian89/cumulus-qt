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

#include "Location.h"
#include "Weather.h"
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
    bool startCon(QSqlDatabase *db);
    bool stopCon(QSqlDatabase *db);
    QSqlDatabase getDatabase();
    QString databaseName(const QString &appName);
    bool databaseInit(QSqlDatabase *db);
    void emitQueryError(const QString &method, const QString &errorString);
public:    
    static struct location_table_definition DB_MA_LOCATION;
    static struct weather_table_definition DB_TR_WEATHER;
    static struct forecast_table_definition DB_TR_FORECAST;
    explicit DatabaseHelper(QObject *parent = nullptr);
    bool clearLocationCode(const int &locationId);            
    Location* getLocation(const int &locationId);
    bool insertLocation(const Location *locationPtr);
    bool updateLocation(const Location *locationPtr);
    bool deleteLocation(const int &locationId);
    Weather* getWeather(const int &locationId);
    bool insertWeather(const Weather *weatherPtr);
    bool updateWeather(const Weather *weatherPtr);
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
