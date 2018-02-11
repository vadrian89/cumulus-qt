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
#include "DatabaseHelper.h"
#include "Location.h"

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
    emit searchStarted();
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

void SearchLocation::setLocationName(const QString &locationName) {
    if (locationName.trimmed().size() > 0 && m_locationName != locationName) {
        timer->stop();
        m_locationName = locationName;
        emit locationNameChanged();
        timer->start(3000);
    }
}

void SearchLocation::setLocationResult(const QStringList &locationResult) {
    if (locationResult.size() > 0 && m_locationResult != locationResult) {
        m_locationResult = locationResult;
        emit locationResultChanged();
    }
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
    QPointer<DatabaseHelper> dbHelperPtr = new DatabaseHelper;
    if (!dbHelperPtr.isNull()) {
        if (dbHelperPtr.data()->deleteLocation(1)) {
            QPointer<Location> locPtr = new Location(nullptr, 1, "", location);
            if (dbHelperPtr.data()->insertLocation(locPtr)) {
                emit locationChanged();
            }
            else {
                setError("Error when saving location!");
            }
        }
        else {
            setError("Error cleaning location table!");
        }
    }
}
