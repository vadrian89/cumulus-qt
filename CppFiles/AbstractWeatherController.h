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
#ifndef ABSTRACTWEATHERCONTROLLER_H
#define ABSTRACTWEATHERCONTROLLER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QLocale>
#include <QSqlDriver>

#include "DataController.h"
#include "DbConnection.h"
#include "Util.h"
class AbstractWeatherController : public QObject {
    Q_OBJECT
protected:
    DataController *dataController;
    DbConnection *db;
    enum OperationData { GetLocationId, GetWeather, GetForecast, GetAstronomy };
    int operationData;
    int locationId;
    QString temperatureUnit;
    QString apiKey;
    QJsonObject weatherObject, forecastObject;
    QString locationCode, locationName;

    QJsonObject nextBranch(const QJsonObject &jsonObject, const QString current) const;
    virtual void saveWeatherToDb(const QJsonObject &jsonObject) = 0;
    virtual void saveForecastToDb(const QJsonObject &jsonObject) = 0;
    bool clearWeather();
    bool clearForecastData();
    bool saveLocation(const QString &code);
public:
    explicit AbstractWeatherController(QObject *parent = 0);
    virtual void searchByLocation(QString &location) = 0;
    virtual void searchBycode(QString &code) = 0;
protected slots:
    virtual void readJsonData(QJsonObject jsonObject) = 0;
    void manageError();
public slots:
    void saveDataToDb();
signals:
    void dataChanged();
    void forecastChanged();
    void saveDataError(QString error);
    void networkError(QString error);
    void dataDownloaded();
};

#endif // ABSTRACTWEATHERCONTROLLER_H
