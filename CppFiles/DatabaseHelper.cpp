#include "DatabaseHelper.h"
#include "Util.h"
#include "Forecast.h"

#include <QDebug>
#include <QTime>

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
        qDebug() << "DatabaseHelper::startCon database error!";
        qDebug() << db->lastError().text();
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
        qDebug() << "DatabaseHelper::databaseInit database error!";
        qDebug() << db->lastError().text();
        return false;
    }
}

QString DatabaseHelper::databaseName(const QString &appName) {
    QString databaseName = appName + "-local.db";
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

bool DatabaseHelper::clearLocationCode(const int &locationId) {
    QSqlDatabase db = getDatabase();
    bool result = false;
    if (startCon(&db)) {
        QSqlQuery q(db);
        QString queryString = "update " + DB_MA_LOCATION.TABLE_NAME +" set " + DB_MA_LOCATION.LOC_CODE_COLUMN
                + " = '' where " + DB_MA_LOCATION.LOC_ID_COLUMN + " = :loc_id";
        q.prepare(queryString);
        q.bindValue(":loc_id", locationId);
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

Location* DatabaseHelper::getLocation(const int &locationId) {
    QSqlDatabase db = getDatabase();
    Location *location = nullptr;
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
                location = new Location(nullptr, locationId, q.value("code").toString(), q.value("name").toString());
            }
            emit querySuccessful();
        }
        else {
            emitQueryError("DatabaseHelper::searchCriteria", q.lastError().text());
        }
    }
    stopCon(&db);
    return location;
}

bool DatabaseHelper::insertLocation(const QPointer<Location> &locationPtr) {
    bool result = false;
    if (!locationPtr.isNull()) {
        QSqlDatabase db = getDatabase();
        if (startCon(&db)) {
            QSqlQuery q(db);
            QString queryString = "insert into " + DB_MA_LOCATION.TABLE_NAME
                    + "("
                    + DB_MA_LOCATION.LOC_ID_COLUMN + ", "
                    + DB_MA_LOCATION.LOC_CODE_COLUMN + ", "
                    + DB_MA_LOCATION.LOC_NAME_COLUMN +
                    ") values ((select count(*)+1 from " + DB_MA_LOCATION.TABLE_NAME + "), :code, :name)";
            q.prepare(queryString);
            q.bindValue(":code", locationPtr.data()->m_locationCode);
            q.bindValue(":name", locationPtr.data()->m_locationName);
            if (q.exec()) {
                emit querySuccessful();
                result = true;
                db.commit();
            }
            else {
                emitQueryError("DatabaseHelper::insertLocation", q.lastError().text());
            }
        }
        stopCon(&db);
    }
    return result;
}

bool DatabaseHelper::updateLocation(const QPointer<Location> &locationPtr) {
    bool result = false;
    if (!locationPtr.isNull()) {
        QSqlDatabase db = getDatabase();
        if (startCon(&db)) {
            QSqlQuery q(db);
            QString queryString = "update "
                    + DB_MA_LOCATION.TABLE_NAME + " set "
                    + DB_MA_LOCATION.LOC_CODE_COLUMN + " = :code, "
                    + DB_MA_LOCATION.LOC_NAME_COLUMN + " = :name where "
                    + DB_MA_LOCATION.LOC_ID_COLUMN + " = :id";
            q.prepare(queryString);
            q.bindValue(":code", locationPtr.data()->m_locationCode);
            q.bindValue(":name", locationPtr.data()->m_locationName);
            q.bindValue(":id", locationPtr.data()->m_locationId);
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
    }
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
        if (q.exec()) {
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

Weather* DatabaseHelper::getWeather(const int &locationId) {
    QSqlDatabase db = getDatabase();
    Weather *weather = nullptr;
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
                weather = new Weather();
                QString weatherIconCode = Util::findFontCode(Util::getWeatherApi(), QString::number(q.value("weather_code").toInt()));
                weather->setWeatherCode(q.value("weather_id").toInt());
                weather->setWeatherIcon(weatherIconCode);
                if (QTime::currentTime() > QTime::fromString(q.value("sunset").toString(), "HH:mm"))
                    weather->setWeatherIcon(Util::findFontCode("night-font", weatherIconCode));
                weather->setWeatherDescription(q.value("description").toString());
                weather->setTemperature(q.value("temperature").toInt());
                weather->setPressure(q.value("pressure").toDouble());
                weather->setHumidity(q.value("humidity").toFloat());
                weather->setWindSpeed(q.value("wind_speed").toInt());
                weather->setWindDegree(q.value("wind_degree").toInt());
                weather->setSunrise(q.value("sunrise").toString());
                weather->setSunset(q.value("sunset").toString());
                weather->setTempMax(q.value("temp_max").toInt());
                weather->setTempMin(q.value("temp_min").toInt());
                weather->setTempUnit(Util::temperatureUnitSymbol());
                weather->setSpeedUnit(Util::speedUnitSymbol());
                weather->setLocationLink(q.value("link").toString());
                weather->setForecastList(getForecast(locationId));
                QPointer<Location> locPtr = getLocation(locationId);
                if (!locPtr.isNull())
                    weather->setLocation(locPtr.data()->m_locationName);
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

bool DatabaseHelper::insertWeather(const QPointer<Weather> &weatherPtr) {
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
        q.bindValue(":code", weatherPtr.data()->weatherCode());
        q.bindValue(":description", weatherPtr.data()->weatherDescription());
        q.bindValue(":temperature", weatherPtr.data()->temperature());
        q.bindValue(":humidity", weatherPtr.data()->humidity());
        q.bindValue(":wind_speed", weatherPtr.data()->windSpeed());
        q.bindValue(":wind_degree", weatherPtr.data()->windDegree());
        q.bindValue(":sunrise", weatherPtr.data()->sunrise());
        q.bindValue(":sunset", weatherPtr.data()->sunset());
        q.bindValue(":pressure", weatherPtr.data()->pressure());
        q.bindValue(":link", weatherPtr.data()->locationLink());
        q.bindValue(":loc_id", weatherPtr.data()->locationId());
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

bool DatabaseHelper::updateWeather(const QPointer<Weather> &weatherPtr) {
    bool result = false;
    QSqlDatabase db = getDatabase();
    if (startCon(&db)) {
        QSqlQuery q(db);
        QString queryString = "update sw_tr_weather set w_weather_code = :code, w_description = :description, "
                              "w_temperature = :temperature, w_humidity = :humidity, w_wind_speed = :wind_speed, "
                              "w_wind_degree = :wind_degree, w_sunrise = :sunrise, w_sunset = :sunset,  "
                              "w_pressure = :pressure, w_link = :link "
                              "where w_loc_id = :loc_id";
        q.prepare(queryString);
        q.bindValue(":code", weatherPtr.data()->weatherCode());
        q.bindValue(":description", weatherPtr.data()->weatherDescription());
        q.bindValue(":temperature", weatherPtr.data()->temperature());
        q.bindValue(":humidity", weatherPtr.data()->humidity());
        q.bindValue(":wind_speed", weatherPtr.data()->windSpeed());
        q.bindValue(":wind_degree", weatherPtr.data()->windDegree());
        q.bindValue(":sunrise", weatherPtr.data()->sunrise());
        q.bindValue(":sunset", weatherPtr.data()->sunset());
        q.bindValue(":pressure", weatherPtr.data()->pressure());
        q.bindValue(":link", weatherPtr.data()->locationLink());
        q.bindValue(":loc_id", weatherPtr.data()->locationId());
        if (q.exec()) {
            emit querySuccessful();
            result = true;
        }
        else {
            emitQueryError("DatabaseHelper::updateWeather", q.lastError().text());
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
            while (q.next()) {
                Forecast *forecast = new Forecast();
                forecast->setForecastIndex(q.value("forec_id").toInt());
                forecast->setWeatherCode(q.value("weather_code").toInt());
                forecast->setWeatherIcon(Util::findFontCode(Util::getWeatherApi(), QString::number(forecast->weatherCode())));
                forecast->setForecastDesc(q.value("description").toString());
                forecast->setTempLow(q.value("temp_min").toInt());
                forecast->setTempHigh(q.value("temp_max").toInt());
                forecast->setForecastDate(q.value("date").toDate().toString("ddd").toUpper().remove("."));
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
