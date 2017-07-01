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
#include "DbConnection.h"
#include "Util.h"
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QDebug>

DbConnection::DbConnection(QObject *parent) : QObject(parent){
    QString appName = QApplication::applicationName();
    db = QSqlDatabase::database();
    if (db.isValid() == false) {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setDatabaseName(databaseName(appName));
}

QSqlDatabase DbConnection::getDatabase() {
    return db;
}

bool DbConnection::startCon() {
    if (db.open()) {
        databaseInit();
        return true;
    }
    else {
        return false;
    }
}
bool DbConnection::stopCon() {
    db.close();
    if (!db.isOpen()) {
        return true;
    }
    else {
        return false;
    }
}

QString DbConnection::getError() {
    return db.lastError().text();
}

bool DbConnection::databaseInit() {
    if (db.isOpen()) {
        QString createLocationTable = "create table if not exists sw_ma_location("
                                      "loc_id int,"
                                      "loc_code varchar(25),"
                                      "loc_name varchar(120)"
                                      ")";

        QString createWeatherTable = "create table if not exists sw_tr_weather("
                                     "w_weather_id int,"
                                     "w_weather_code int,"
                                     "w_description varchar(100),"
                                     "w_temperature decimal(5,2),"
                                     "w_pressure decimal(5,2),"
                                     "w_humidity decimal(5,2),"
                                     "w_wind_speed decimal(5,2),"
                                     "w_wind_degree decimal(10,3),"
                                     "w_sunrise varchar(8),"
                                     "w_sunset varchar(8),"
                                     "w_link text,"
                                     "w_loc_id int"
                                     ")";
        QString createForecastTable = "create table if not exists sw_tr_forecast("
                                      "forec_id int,"
                                      "forec_date date,"
                                      "forec_weather_code varchar(25),"
                                      "forec_weather_description varchar(100),"
                                      "forec_temp_min decimal(5,2),"
                                      "forec_temp_max decimal(5,2),"
                                      "forec_pressure decimal(5,2),"
                                      "forec_humidity decimal(5,2),"
                                      "forec_wind_speed decimal(5,2),"
                                      "forec_wind_degree decimal(10,3),"
                                      "forec_loc_id int"
                                      ")";
        QSqlQuery q;
        q.prepare(createLocationTable);
        if(q.exec()) {
            q.prepare(createWeatherTable);
            if (q.exec()) {
                q.prepare(createForecastTable);
                if (q.exec() ) {
                    return true;
                }
                else {
                    qDebug() << "createForecastTable query failed.";
                    qDebug() << q.lastError().text();
                    return false;
                }
            }
            else {
                qDebug() << "createWeatherTable query failed.";
                qDebug() << q.lastError().text();
                return false;
            }
        }
        else {
            qDebug() << "createLocationTable query failed.";
            qDebug() << q.lastError().text();
            return false;
        }
    }
    else {
        qDebug() << this->getError();
        return false;
    }
}

QString DbConnection::databaseName(const QString &appName) {
    QString databaseName = appName + "-local.db";
    if (QSysInfo::kernelType() == "linux" && QSysInfo::productType() != "android") {
        moveDatabaseFile(databaseName);
        return QDir::homePath() + "/." + databaseName;
    }
    else {
        return databaseName;
    }
}

/*
 * Method to move present database files into $HOME, to ensure compatibility with older versions,
 * newer versions and Appimage packaged versions.
 * TODO; To be removed after 2nd application update, just to be safe
*/
void DbConnection::moveDatabaseFile(const QString &fileName) {
    QString oldFile = QApplication::applicationDirPath() + "/" + fileName;
    QString newFile = QDir::homePath() + "/." + fileName;
    if (QFile::exists(oldFile) && !QFile::exists(newFile)) {
        QFile::copy(oldFile, newFile);
    }
}
