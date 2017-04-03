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
#include "SearchLocation.h"
#include "DbConnection.h"

#include <QDebug>
SearchLocation::SearchLocation(QObject *parent) : QObject(parent) {
    dataController = new DataController(this);
    connect(dataController, SIGNAL(jsonObjectReady(QJsonObject)), this, SLOT(readJsonData(QJsonObject)));
    connect(dataController, SIGNAL(networkError()), this, SLOT(errorSlot()));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(doSearch()));
}

void SearchLocation::doSearch() {
    timer->stop();
    dataController->getDataFromUrl("http://autocomplete.wunderground.com/aq?query=" + m_locationName);
}

void SearchLocation::readJsonData(QJsonObject jsonObject) {
    QJsonArray array = jsonObject.find("RESULTS").value().toArray();
    QStringList stringList;
    for (int i = 0; i < array.size(); i++) {
        stringList.append(array.at(i).toObject().find("name").value().toString());
    }
    setLocationResult(stringList);
}

QJsonObject SearchLocation::nextBranch(const QJsonObject jsonObject, const QString current) const {
    return jsonObject.find(current).value().toObject();
}

QString SearchLocation::locationName() const{
    return m_locationName;
}

void SearchLocation::setLocationName(const QString &locationName) {
    if (locationName.trimmed().size() > 0 && m_locationName != locationName) {
        timer->stop();
        m_locationName = locationName;
        emit locationNameChanged();
        timer->start(3000);
    }
}

QStringList SearchLocation::locationResult() const{
    return m_locationResult;
}

void SearchLocation::setLocationResult(const QStringList &locationResult) {
    if (locationResult.size() > 0 && m_locationResult != locationResult) {
        m_locationResult = locationResult;
        emit locationResultChanged();
    }
}

QString SearchLocation::error() const{
    return m_error;
}

void SearchLocation::setError(const QString &error) {
    if (error.trimmed().size() > 0 && m_error != error) {
        m_error = error;
        emit errorChanged();
    }
}

void SearchLocation::errorSlot() {
    setError(dataController->managerError());
}

void SearchLocation::setLocation(const QString &location) {
    DbConnection *con = new DbConnection(this);
    QString deleteQ = "delete from sw_ma_location";
    QString insertQ = "insert into sw_ma_location (loc_id, loc_name) values ("
                      "(select count(*)+1 from sw_ma_location), "
                      ":location"
                      ")";
    if(con->startCon()) {
        QSqlQuery q;
        q.prepare(deleteQ);
        if(q.exec()) {
            q.prepare(insertQ);
            q.bindValue(":location", location);
            if(q.exec()) {
                emit locationChanged();
            }
            else {
                setError("Sql error when saving location: " + q.lastError().text());
            }
        }
        else {
            setError("Error cleaning location table: " + q.lastError().text());
        }
    }
    else {
        setError("Database error: " + con->getError());
    }
}
