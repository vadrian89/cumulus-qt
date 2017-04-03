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
#include "Forecast.h"
#include "DbConnection.h"
#include <QDebug>

Forecast::Forecast(QObject *parent) : QObject(parent){
	m_forecastIndex = 1;
    m_weatherCode = "-1";
	m_tempHigh = 0;
	m_tempLow  = 0;
    m_forecastDate = QDate::currentDate().toString("ddd");
}

void Forecast::setForecastIndex(const int &forecastIndex) {
	if ( m_forecastIndex != forecastIndex ) {
		m_forecastIndex = forecastIndex;
		emit forecastIndexChanged();
	}
}

QString Forecast::weatherCode() const {
    return m_weatherCode;
}

void Forecast::setWeatherCode(const QString &weatherCode) {
	if ( m_weatherCode != weatherCode ) {
		m_weatherCode = weatherCode;
		emit weatherCodeChanged();
	}
}

void Forecast::setTempHigh(const int &tempHigh) {
	if ( m_tempHigh != tempHigh ) {
		m_tempHigh = tempHigh;
		emit tempHighChanged();
	}
}

void Forecast::setTempLow(const int &tempLow) {
	if ( m_tempLow != tempLow ) {
		m_tempLow = tempLow;
		emit tempLowChanged();
	}
}

void Forecast::setForecastDate(const QString &forecastDate) {
	if ( m_forecastDate != forecastDate ) {
		m_forecastDate = forecastDate;
		emit forecastDateChanged();
	}
}

int Forecast::forecastIndex() const {
	return m_forecastIndex;
}
int Forecast::tempHigh() const {
	return m_tempHigh;
}
int Forecast::tempLow() const {
	return m_tempLow;
}
QString Forecast::forecastDate() const {
	return m_forecastDate;
}

QString Forecast::forecastDesc() const {
    return m_forecastDesc;
}

void Forecast::setForecastDesc(const QString &forecastDesc) {
    if (m_forecastDesc != forecastDesc) {
        m_forecastDesc = forecastDesc;
        emit forecastDescChanged();
    }
}
