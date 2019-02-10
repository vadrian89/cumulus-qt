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
#include <QMap>
#include <memory>
#include <QRegExp>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

#include "DataController.h"
#include "Util.h"
#include "DatabaseHelper.h"
#include "Forecast.h"
#include "SettingsController.h"

using namespace std;

class AbstractWeatherController : public QObject {
    Q_OBJECT
protected:
    DataController *dataController;
    QString temperatureUnit, apiKey, locationCode;
    weather_struct m_weather;

    QJsonObject nextBranch(const QJsonObject &jsonObject, const QString current) const;
    bool saveWeather(const weather_struct &weather);
    void saveForecast(QList<Forecast*> &forecastList);
    bool saveLocation(const QString &code);
    bool saveLocation(const QString &code, const QString &name);
public:
    QString name;
    explicit AbstractWeatherController(QObject *parent = nullptr);
    virtual void searchByLocation(QString &location) = 0;
    virtual void searchBycode(QString &code) = 0;
    virtual void searchByGps(const double &lat, const double &lon) = 0;
private slots:
    void locationPositionInfo(const QGeoPositionInfo &posInfo);
    void locationPositionError(const QGeoPositionInfoSource::Error &positioningError);
protected slots:
    virtual void getLocationFromJson(const QJsonObject &jsonObject) = 0;
    virtual void getWeatherFromJson(const QJsonObject &jsonObject) = 0;
    virtual void getForecastFromJson(const QJsonObject &jsonObject) = 0;
    void manageError(const QString error);
public slots:
    void getWeather();
signals:
    void forecastChanged();
    void saveDataError(const QString &error);
    void networkError(const QString &error);
    void weatherSet(const weather_struct &weather);
};

#endif // ABSTRACTWEATHERCONTROLLER_H
