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
#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QByteArray>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QUrl>

class DataController : public QObject {
    Q_OBJECT
    QNetworkAccessManager *networkManager;
    QNetworkReply *networkReply;
    QNetworkRequest networkRequest;
    QJsonObject receivedData;
public:
    explicit DataController(QObject *parent = 0);
    void getDataFromUrl(QString urlString);
    QString managerError() const;
public slots:    
private slots:
    void readFinished();
    void networkError(QNetworkReply::NetworkError);
signals:
    void jsonObjectReady(QJsonObject);
    void networkError();
};

#endif // DATACONTROLLER_H
