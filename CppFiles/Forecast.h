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
#ifndef FORECAST_H
#define FORECAST_H

#include <QObject>
#include <QDate>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>

class Forecast : public QObject {
    Q_OBJECT
    int m_forecastIndex;
    QString m_weatherCode;
    int m_tempHigh;
    int m_tempLow;
    QString m_forecastDate;
    QString m_forecastDesc;

    Q_PROPERTY(int forecastIndex READ forecastIndex WRITE setForecastIndex NOTIFY forecastIndexChanged)
    Q_PROPERTY(QString weatherCode READ weatherCode WRITE setWeatherCode NOTIFY weatherCodeChanged)
    Q_PROPERTY(int tempHigh READ tempHigh WRITE setTempHigh NOTIFY tempHighChanged)
    Q_PROPERTY(int tempLow READ tempLow WRITE setTempLow NOTIFY tempLowChanged)
    Q_PROPERTY(QString forecastDate READ forecastDate WRITE setForecastDate NOTIFY forecastDateChanged)
    Q_PROPERTY(QString forecastDesc READ forecastDesc WRITE setForecastDesc NOTIFY forecastDescChanged)
public:
    explicit Forecast(QObject *parent = 0);
    void setForecastIndex(const int &forecastIndex);
    void setWeatherCode(const QString &weatherCode);
    void setTempHigh(const int &tempHigh);
    void setTempLow(const int &tempLow);
    void setForecastDate(const QString &forecastDate);
	
    int forecastIndex() const;
    QString weatherCode() const;
    int tempHigh() const;
    int tempLow() const;
    QString forecastDate() const;
    QString forecastDesc() const;
    void setForecastDesc(const QString &desc);
private slots:
signals:
    void forecastIndexChanged();
    void weatherCodeChanged();
	void tempHighChanged();
	void tempLowChanged();
	void forecastDateChanged();
	void forecastChanged();
    void forecastDescChanged();
};

#endif // FORECAST_H
