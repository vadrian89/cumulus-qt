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
#include "DatabaseHelper.h"
#include "Util.h"
#include "Forecast.h"

location_table_definition DatabaseHelper::DB_MA_LOCATION;
weather_table_definition DatabaseHelper::DB_TR_WEATHER;
forecast_table_definition DatabaseHelper::DB_TR_FORECAST;

DatabaseHelper::DatabaseHelper(QObject *parent) : QObject(parent) {}

QString DatabaseHelper::locationTableCreationQuery() {
    QString def = "create table if not exists ";
    def.append(DB_MA_LOCATION.TABLE_NAME).append("(")
            .append(DB_MA_LOCATION.LOC_ID_COLUMN).append(" int,")
            .append(DB_MA_LOCATION.LOC_CODE_COLUMN).append(" varchar(25),")
            .append(DB_MA_LOCATION.LOC_NAME_COLUMN).append(" varchar(120))");
    return def;
}

QString DatabaseHelper::weatherTableCreationQuery() {
    QString def = "create table if not exists ";
    def.append(DB_TR_WEATHER.TABLE_NAME).append("(")
            .append(DB_TR_WEATHER.WEATHER_ID_COLUMN).append(" int,")
            .append(DB_TR_WEATHER.WEATHER_CODE_COLUMN).append(" int,")
            .append(DB_TR_WEATHER.WEATHER_DESCRIPTION_COLUMN).append(" text,")
            .append(DB_TR_WEATHER.WEATHER_TEMPERATURE_COLUMN).append(" decimal(5,2),")
            .append(DB_TR_WEATHER.WEATHER_PRESSURE_COLUMN).append(" decimal(5,2),")
            .append(DB_TR_WEATHER.WEATHER_HUMIDITY_COLUMN).append(" int,")
            .append(DB_TR_WEATHER.WEATHER_WIND_SPEED_COLUMN).append(" decimal(5,2),")
            .append(DB_TR_WEATHER.WEATHER_WIND_DEGREE_COLUMN).append(" decimal(10,3),")
            .append(DB_TR_WEATHER.WEATHER_SUNRISE_COLUMN).append(" varchar(8),")
            .append(DB_TR_WEATHER.WEATHER_SUNSET_COLUMN).append(" varchar(8),")
            .append(DB_TR_WEATHER.WEATHER_LINK_COLUMN).append(" text,")
            .append(DB_TR_WEATHER.WEATHER_LOC_ID_COLUMN).append(" int)");
    return def;
}

QString DatabaseHelper::forecastTableCreationQuery() {
    QString def = "create table if not exists ";
    def.append(DB_TR_FORECAST.TABLE_NAME).append("(")
            .append(DB_TR_FORECAST.FOREC_ID_COLUMN).append(" int,")
            .append(DB_TR_FORECAST.FOREC_DATE_COLUMN).append(" date,")
            .append(DB_TR_FORECAST.FOREC_WEATHER_CODE_COLUMN).append(" int,")
            .append(DB_TR_FORECAST.FOREC_WEATHER_DESCRIPTION_COLUMN).append(" text,")
            .append(DB_TR_FORECAST.FOREC_TEMP_MIN_COLUMN).append(" decimal(5,2),")
            .append(DB_TR_FORECAST.FOREC_TEMP_MAX_COLUMN).append(" decimal(5,2),")
            .append(DB_TR_FORECAST.FOREC_PRESSURE_COLUMN).append(" decimal(5,2),")
            .append(DB_TR_FORECAST.FOREC_HUMIDITY_COLUMN).append(" int,")
            .append(DB_TR_FORECAST.FOREC_WIND_SPEED_COLUMN).append(" decimal(5,2),")
            .append(DB_TR_FORECAST.FOREC_WIND_DEGREE_COLUMN).append(" decimal(10,3),")
            .append(DB_TR_FORECAST.FOREC_LOC_ID_COLUMN).append(" int)");
    return def;
}

QSqlDatabase DatabaseHelper::getDatabase() {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid()) db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databaseName(QApplication::applicationName()));
    return db;
}

bool DatabaseHelper::startCon(QSqlDatabase *db) {
    if (db->open()) {
        databaseInit(db);
        return true;
    }
    else {
        emitQueryError("DatabaseHelper::startCon database error!", db->lastError().text());
        return false;
    }
}
bool DatabaseHelper::stopCon(QSqlDatabase *db) {
    db->close();
    if (!db->isOpen())
        return true;
    else
        return false;
}

bool DatabaseHelper::databaseInit(QSqlDatabase *db) {
    if (db->isOpen()) {
        QSqlQuery q(getDatabase());
        q.prepare(locationTableCreationQuery());
        if(q.exec()) {
            q.prepare(weatherTableCreationQuery());
            if (q.exec()) {
                q.prepare(forecastTableCreationQuery());
                if (q.exec() ) {
                    return true;
                }
                else {
                    emitQueryError("DatabaseHelper::databaseInit", q.lastError().text());
                    return false;
                }
            }
            else {
                emitQueryError("DatabaseHelper::databaseInit", q.lastError().text());
                return false;
            }
        }
        else {
            emitQueryError("DatabaseHelper::databaseInit", q.lastError().text());
            return false;
        }
    }
    else {
        emitQueryError("DatabaseHelper::databaseInit database error", db->lastError().text());
        return false;
    }
}

QString DatabaseHelper::databaseName(const QString &appName) {
    QString databaseName = appName + "-local-v" + QString::number(SettingsController::SETTINGS_VERSION) + ".db";
    if (QSysInfo::kernelType() == "linux" && QSysInfo::productType() == "android")
        return databaseName;
    else
        return QDir::homePath() + "/." + databaseName;
}

void DatabaseHelper::emitQueryError(const QString &method, const QString &errorString) {
    QSqlDatabase db = getDatabase();
    stopCon(&db);
    qDebug() << method + " query failed: " << errorString;
    emit queryError(errorString);
}

bool DatabaseHelper::clearLocationCode() {
    QSqlDatabase db = getDatabase();
    bool result = false;
    if (startCon(&db)) {
        QSqlQuery q(db);
        QString queryString = "update " + DB_MA_LOCATION.TABLE_NAME +" set " + DB_MA_LOCATION.LOC_CODE_COLUMN
                + " = ''";
        q.prepare(queryString);
        if (q.exec()) {
            emit querySuccessful();
            stopCon(&db);
            result = true;
        }
        else {
            emitQueryError("DatabaseHelper::clearLocationCode", q.lastError().text());
        }
    }
    return result;
}

location_struct DatabaseHelper::getLocation(const int &locationId) {
    QSqlDatabase db = getDatabase();
    location_struct location;
    location.m_locationId = -1;
    if (startCon(&db)) {
        QSqlQuery q(db);
        QString queryString = "select "
                + DB_MA_LOCATION.LOC_ID_COLUMN + " as id, "
                + DB_MA_LOCATION.LOC_CODE_COLUMN + " as code, "
                + DB_MA_LOCATION.LOC_NAME_COLUMN + " as name from "
                + DB_MA_LOCATION.TABLE_NAME + " where " + DB_MA_LOCATION.LOC_ID_COLUMN + " = :loc_id";
        q.prepare(queryString);
        q.bindValue(":loc_id", locationId);
        if (q.exec()) {
            if (q.next()) {
                location.m_locationId = locationId;
                location.m_locationCode = q.value("code").toString();
                location.m_locationName = q.value("name").toString();
            }
            emit querySuccessful();
        }
        else {
            emitQueryError("DatabaseHelper::getLocation", q.lastError().text());
        }
    }
    stopCon(&db);
    return location;
}

location_struct DatabaseHelper::findLocation(const QString &locationName) {
    QSqlDatabase db = getDatabase();
    location_struct location;
    location.m_locationId = -1;
    if (startCon(&db)) {
        QSqlQuery q(db);
        QString queryString = "select "
                + DB_MA_LOCATION.LOC_ID_COLUMN + " as id, "
                + DB_MA_LOCATION.LOC_CODE_COLUMN + " as code, "
                + DB_MA_LOCATION.LOC_NAME_COLUMN + " as name from "
                + DB_MA_LOCATION.TABLE_NAME + " where " + DB_MA_LOCATION.LOC_NAME_COLUMN + " = :loc_name";
        q.prepare(queryString);
        q.bindValue(":loc_name", locationName);
        if (q.exec()) {
            if (q.next()) {
                location.m_locationId = q.value("id").toInt();
                location.m_locationCode = q.value("code").toString();
                location.m_locationName = q.value("name").toString();
            }
            emit querySuccessful();
        }
        else {
            emitQueryError("DatabaseHelper::getLocation", q.lastError().text());
        }
    }
    stopCon(&db);
    return location;
}

QList<location_struct> DatabaseHelper::getLocationList() {
    QSqlDatabase db = getDatabase();
    QList<location_struct> list;
    if (startCon(&db)) {
        QSqlQuery q(db);
        QString queryString = "select "
                + DB_MA_LOCATION.LOC_ID_COLUMN + " as id, "
                + DB_MA_LOCATION.LOC_CODE_COLUMN + " as code, "
                + DB_MA_LOCATION.LOC_NAME_COLUMN + " as name from "
                + DB_MA_LOCATION.TABLE_NAME;
        q.prepare(queryString);
        if (q.exec()) {
            while (q.next()) {
                location_struct location;
                location.m_locationId = q.value("id").toInt();
                location.m_locationCode = q.value("code").toString();
                location.m_locationName = q.value("name").toString();
                list.append(location);
            }
            emit querySuccessful();
        }
        else {
            emitQueryError("DatabaseHelper::getLocationList", q.lastError().text());
        }
    }
    stopCon(&db);
    return list;
}

int DatabaseHelper::lastLocationId() {
    int result = -1;
    QSqlDatabase db = getDatabase();
    if (startCon(&db)) {
        QSqlQuery q(db);
        QString queryString = "select "
                + DB_MA_LOCATION.LOC_ID_COLUMN + "+1 as loc_id"
                + " from " + DB_MA_LOCATION.TABLE_NAME
                + " order by " + DB_MA_LOCATION.LOC_ID_COLUMN
                + " desc limit 1";
        q.prepare(queryString);
        q.exec();
        if (q.next())
            result = q.value("loc_id").toInt();
    }
    stopCon(&db);
    return result;
}

bool DatabaseHelper::insertLocation(const location_struct &location) {
    bool result = false;
    QSqlDatabase db = getDatabase();
    int id = lastLocationId();
    if (id == -1) id = 1;
    if (startCon(&db)) {
        QSqlQuery q(db);
        QString queryString = "insert into " + DB_MA_LOCATION.TABLE_NAME
                + "("
                + DB_MA_LOCATION.LOC_ID_COLUMN + ", "
                + DB_MA_LOCATION.LOC_CODE_COLUMN + ", "
                + DB_MA_LOCATION.LOC_NAME_COLUMN +
                ") values (:id, :code, :name)";
        q.prepare(queryString);
        q.bindValue(":id", id);
        q.bindValue(":code", location.m_locationCode);
        q.bindValue(":name", location.m_locationName);
        if (q.exec()) {
            emit querySuccessful();
            result = true;
            db.commit();
            SettingsController settings;
            if (settings.currentLocationId() == -1)
                settings.setCurrentLocationId(id);
        }
        else {
            emitQueryError("DatabaseHelper::insertLocation", q.lastError().text());
        }
    }
    stopCon(&db);
    return result;
}

bool DatabaseHelper::updateLocation(const location_struct &location) {
    bool result = false;
    QSqlDatabase db = getDatabase();
    if (startCon(&db)) {
        QSqlQuery q(db);
        QString queryString = "update "
                + DB_MA_LOCATION.TABLE_NAME + " set "
                + DB_MA_LOCATION.LOC_CODE_COLUMN + " = :code, "
                + DB_MA_LOCATION.LOC_NAME_COLUMN + " = :name where "
                + DB_MA_LOCATION.LOC_ID_COLUMN + " = :id";
        q.prepare(queryString);
        q.bindValue(":code", location.m_locationCode);
        q.bindValue(":name", location.m_locationName);
        q.bindValue(":id", location.m_locationId);
        if (q.exec()) {
            emit querySuccessful();
            result = true;
            db.commit();
        }
        else {
            emitQueryError("DatabaseHelper::updateLocation", q.lastError().text());
        }
    }
    stopCon(&db);
    return result;
}

bool DatabaseHelper::deleteLocation(const int &locationId) {
    bool result = false;
    QSqlDatabase db = getDatabase();
    if (startCon(&db)) {
        QSqlQuery q(db);
        QString queryString = "delete from " + DB_MA_LOCATION.TABLE_NAME +
                " where " + DB_MA_LOCATION.LOC_ID_COLUMN + " = :id";
        q.prepare(queryString);
        q.bindValue(":id", locationId);
        if (q.exec() && deleteWeather(locationId) && deleteForecast(locationId)) {
            emit querySuccessful();
            result = true;
            db.commit();
        }
        else {
            emitQueryError("DatabaseHelper::deleteLocation", q.lastError().text());
        }
    }
    stopCon(&db);
    return result;
}

weather_struct DatabaseHelper::getWeather(const int &locationId) {
    QSqlDatabase db = getDatabase();
    weather_struct weather;
    if (startCon(&db)) {
        QSqlQuery q(db);
        QString queryString = "select "
                + DB_TR_WEATHER.WEATHER_ID_COLUMN + " as weather_id, "
                + DB_TR_WEATHER.WEATHER_CODE_COLUMN + " as weather_code, "
                + DB_TR_WEATHER.WEATHER_DESCRIPTION_COLUMN + " as description, "
                + DB_TR_WEATHER.WEATHER_TEMPERATURE_COLUMN + " as temperature, "
                + DB_TR_WEATHER.WEATHER_PRESSURE_COLUMN + " as pressure, "
                + DB_TR_WEATHER.WEATHER_HUMIDITY_COLUMN + " as humidity, "
                + DB_TR_WEATHER.WEATHER_WIND_SPEED_COLUMN + " as wind_speed, "
                + DB_TR_WEATHER.WEATHER_WIND_DEGREE_COLUMN + " as wind_degree, "
                + DB_TR_WEATHER.WEATHER_SUNRISE_COLUMN + " as sunrise, "
                + DB_TR_WEATHER.WEATHER_SUNSET_COLUMN + " as sunset, "
                                                        " (select "
                + DB_TR_FORECAST.FOREC_TEMP_MIN_COLUMN + " from "
                + DB_TR_FORECAST.TABLE_NAME + " where "
                + DB_TR_FORECAST.FOREC_LOC_ID_COLUMN + " = :loc_id order by "
                + DB_TR_FORECAST.FOREC_DATE_COLUMN + " limit 1) as temp_min, "
                                                     "(select "
                + DB_TR_FORECAST.FOREC_TEMP_MAX_COLUMN + " from "
                + DB_TR_FORECAST.TABLE_NAME + " where "
                + DB_TR_FORECAST.FOREC_LOC_ID_COLUMN + " = :loc_id order by "
                + DB_TR_FORECAST.FOREC_DATE_COLUMN + " limit 1) as temp_max, "
                + DB_TR_WEATHER.WEATHER_LINK_COLUMN + " as link "
                                                      "from "
                + DB_TR_WEATHER.TABLE_NAME + " where "
                + DB_TR_WEATHER.WEATHER_LOC_ID_COLUMN + " = :loc_id";
        q.prepare(queryString);
        q.bindValue(":loc_id", locationId);
        if (q.exec()) {
            if (q.next()) {
                weather.m_weatherCode = q.value("weather_code").toInt();
                weather.m_weatherDescription = q.value("description").toString();
                weather.m_temperature = q.value("temperature").toInt();
                weather.m_pressure = q.value("pressure").toDouble();
                weather.m_humidity = q.value("humidity").toFloat();
                weather.m_windSpeed = q.value("wind_speed").toInt();
                weather.m_windDegree = q.value("wind_degree").toInt();
                weather.m_sunrise = q.value("sunrise").toString();
                weather.m_sunset = q.value("sunset").toString();
                weather.m_tempMax = q.value("temp_max").toInt();
                weather.m_tempMin = q.value("temp_min").toInt();
                weather.m_locationLink = q.value("link").toString();
                weather.m_forecastList = getForecast(locationId);
                weather.m_location = getLocation(locationId).m_locationName;
            }
            emit querySuccessful();
        }
        else {
            emitQueryError("DatabaseHelper::getWeather", q.lastError().text());
        }
    }
    stopCon(&db);
    return weather;
}

bool DatabaseHelper::insertWeather(const weather_struct &weather) {
    bool result = false;
    QSqlDatabase db = getDatabase();
    if (startCon(&db)) {
        QSqlQuery q(db);
        QString queryString = "insert into "
                + DB_TR_WEATHER.TABLE_NAME + " ("
                + DB_TR_WEATHER.WEATHER_ID_COLUMN + ", "
                + DB_TR_WEATHER.WEATHER_CODE_COLUMN + ", "
                + DB_TR_WEATHER.WEATHER_DESCRIPTION_COLUMN + ", "
                + DB_TR_WEATHER.WEATHER_TEMPERATURE_COLUMN + ", "
                + DB_TR_WEATHER.WEATHER_HUMIDITY_COLUMN + ", "
                + DB_TR_WEATHER.WEATHER_WIND_SPEED_COLUMN + ", "
                + DB_TR_WEATHER.WEATHER_WIND_DEGREE_COLUMN + ", "
                + DB_TR_WEATHER.WEATHER_SUNRISE_COLUMN + ", "
                + DB_TR_WEATHER.WEATHER_SUNSET_COLUMN + ", "
                + DB_TR_WEATHER.WEATHER_LOC_ID_COLUMN + ", "
                + DB_TR_WEATHER.WEATHER_PRESSURE_COLUMN + ", "
                + DB_TR_WEATHER.WEATHER_LINK_COLUMN + ")";
        queryString.append("values ((select count(*)+1 from "
                           + DB_TR_WEATHER.TABLE_NAME + "), :code, :description, :temperature, "
                                                        ":humidity, :wind_speed, :wind_degree, :sunrise, :sunset, :loc_id, :pressure, :link)");
        q.prepare(queryString);
        q.bindValue(":code", weather.m_weatherCode);
        q.bindValue(":description", weather.m_weatherDescription);
        q.bindValue(":temperature", weather.m_temperature);
        q.bindValue(":humidity", weather.m_humidity);
        q.bindValue(":wind_speed", weather.m_windSpeed);
        q.bindValue(":wind_degree", weather.m_windDegree);
        q.bindValue(":sunrise", weather.m_sunrise);
        q.bindValue(":sunset", weather.m_sunset);
        q.bindValue(":pressure", weather.m_pressure);
        q.bindValue(":link", weather.m_locationLink);
        q.bindValue(":loc_id", weather.m_locationId);
        if (q.exec()) {
            emit querySuccessful();
            result = true;
        }
        else {
            emitQueryError("DatabaseHelper::insertWeather", q.lastError().text());
        }
    }
    stopCon(&db);
    return result;
}

bool DatabaseHelper::deleteWeather(const int &locationId) {
    bool result = false;
    QSqlDatabase db = getDatabase();
    if (startCon(&db)) {
        QSqlQuery q(db);
        QString queryString = "delete from " + DB_TR_WEATHER.TABLE_NAME + " where " + DB_TR_WEATHER.WEATHER_LOC_ID_COLUMN + " = :id";
        q.prepare(queryString);
        q.bindValue(":id", locationId);
        if (q.exec()) {
            emit querySuccessful();
            result = true;
        }
        else {
            emitQueryError("DatabaseHelper::deleteWeather", q.lastError().text());
        }
    }
    stopCon(&db);
    return result;
}

QList<QObject*> DatabaseHelper::getForecast(const int &locationId) {
    QSqlDatabase db = getDatabase();
    QList<QObject*> list;
    if (startCon(&db)) {
        QSqlQuery q(db);
        QString queryString = "select "
                + DB_TR_FORECAST.FOREC_ID_COLUMN + " as forec_id, "
                + DB_TR_FORECAST.FOREC_DATE_COLUMN + " as date, "
                + DB_TR_FORECAST.FOREC_WEATHER_CODE_COLUMN + " as weather_code, "
                + DB_TR_FORECAST.FOREC_WEATHER_DESCRIPTION_COLUMN + " as description, "
                + DB_TR_FORECAST.FOREC_TEMP_MIN_COLUMN + " as temp_min, "
                + DB_TR_FORECAST.FOREC_TEMP_MAX_COLUMN + " as temp_max "
                                                         "from " + DB_TR_FORECAST.TABLE_NAME + " where " + DB_TR_FORECAST.FOREC_LOC_ID_COLUMN + " = :loc_id";
        q.prepare(queryString);
        q.bindValue(":loc_id", locationId);
        if (q.exec()) {
            SettingsController settings;
            while (q.next()) {
                Forecast *forecast = new Forecast();                
                forecast->setForecastIndex(q.value("forec_id").toInt());
                forecast->setWeatherCode(q.value("weather_code").toInt());
                forecast->setWeatherIcon(Util::findFontCode(settings.weatherApi(), QString::number(forecast->weatherCode())));
                forecast->setForecastDesc(q.value("description").toString());
                forecast->setTempLow(q.value("temp_min").toInt());
                forecast->setTempHigh(q.value("temp_max").toInt());
                forecast->setForecastDate(q.value("date").toString());
                list.append(forecast);
            }
            emit querySuccessful();
        }
        else {
            emitQueryError("DatabaseHelper::getForecast", q.lastError().text());
        }
    }
    stopCon(&db);
    return list;
}

bool DatabaseHelper::insertForecast(const QList<Forecast*> &forecastList) {
    bool result = false;
    if (!forecastList.isEmpty()) {
        if (deleteForecast(forecastList.at(0)->locationId())) {
            QSqlDatabase db = getDatabase();
            if (startCon(&db)) {
                if (db.driver()->hasFeature(QSqlDriver::Transactions)) {
                    db.driver()->beginTransaction();
                }
                for (Forecast *forecast : forecastList) {
                    if (!insertForecast(forecast, db)) {
                        emitQueryError("DatabaseHelper::insertForecast", db.lastError().text());
                        stopCon(&db);
                        return false;
                    }
                }
                emit querySuccessful();
                result = true;
                if (db.driver()->hasFeature(QSqlDriver::Transactions)) {
                    db.driver()->commitTransaction();
                }
            }
            stopCon(&db);
        }
    }
    return result;
}

bool DatabaseHelper::insertForecast(const Forecast *forecast, const QSqlDatabase &db) {
    bool result = false;
    if (forecast != nullptr && db.isOpen()) {
        QString queryString = "insert into " + DB_TR_FORECAST.TABLE_NAME + "("
                + DB_TR_FORECAST.FOREC_ID_COLUMN + ", "
                + DB_TR_FORECAST.FOREC_DATE_COLUMN+ ", "
                + DB_TR_FORECAST.FOREC_WEATHER_CODE_COLUMN + ", "
                + DB_TR_FORECAST.FOREC_WEATHER_DESCRIPTION_COLUMN + ", "
                + DB_TR_FORECAST.FOREC_TEMP_MIN_COLUMN + ", "
                + DB_TR_FORECAST.FOREC_TEMP_MAX_COLUMN + ", "
                + DB_TR_FORECAST.FOREC_LOC_ID_COLUMN + ") "
                "values ((select count(*) from "
                + DB_TR_FORECAST.TABLE_NAME + "), :date ,:code, :description, :temp_min, :temp_max, :loc_id)";
        QSqlQuery q(db);
        q.prepare(queryString);
        q.bindValue(":date", QDate::fromString(forecast->forecastDate(), "dd/MMM/yyyy"));
        q.bindValue(":code", forecast->weatherCode());
        q.bindValue(":description", forecast->forecastDesc());
        q.bindValue(":temp_min", forecast->tempLow());
        q.bindValue(":temp_max", forecast->tempHigh());
        q.bindValue(":loc_id", forecast->locationId());
        if (q.exec())
            result = true;
    }
    return result;
}

bool DatabaseHelper::deleteForecast(const int &locationId) {
    bool result = false;
    QSqlDatabase db = getDatabase();
    if (startCon(&db)) {
        QSqlQuery q(db);
        QString queryString = "delete from "
                + DB_TR_FORECAST.TABLE_NAME + " where " + DB_TR_FORECAST.FOREC_LOC_ID_COLUMN + " = :id";
        q.prepare(queryString);
        q.bindValue(":id", locationId);
        if (q.exec()) {
            emit querySuccessful();
            result = true;
        }
        else {
            emitQueryError("DatabaseHelper::deleteForecast", q.lastError().text());
        }
    }
    stopCon(&db);
    return result;
}
