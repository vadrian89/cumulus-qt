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
#ifndef SEARCHLOCATION_H
#define SEARCHLOCATION_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QTimer>
#include <QSqlQuery>
#include <QSqlError>
#include <memory>

#include "DataController.h"

using namespace std;
class SearchLocation : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString locationName MEMBER m_locationName WRITE setLocationName NOTIFY locationNameChanged)
    Q_PROPERTY(QStringList locationResult MEMBER m_locationResult WRITE setLocationResult NOTIFY locationResultChanged)
    Q_PROPERTY(QString error MEMBER m_error WRITE setError NOTIFY errorChanged)
    QString m_locationName, m_error;
    QStringList m_locationResult;
    QTimer *timer;
public:
    explicit SearchLocation(QObject *parent = 0);    
    QString locationName() const;
    QStringList locationResult() const;
    QString error() const;

    void setLocationName(const QString &locationName);
    void setLocationResult(const QStringList &locationResult);
    void setError(const QString &error);    
private:
    DataController *dataController;
    QJsonObject nextBranch(const QJsonObject jsonObject, const QString current) const;    
public slots:
    void setLocation(const QString &location);
private slots:    
    void readJsonData(QJsonObject jsonObject);
    void doSearch();
    void errorSlot(const QString &error);
signals:
    void locationNameChanged();
    void locationResultChanged();
    void errorChanged();
    void locationChanged();
    void searchStarted();
};

#endif // SEARCHLOCATION_H
