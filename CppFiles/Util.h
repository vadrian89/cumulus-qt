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
#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <QMap>
#include <QSettings>
#include <QPoint>
#include <QSize>
#include <QImage>
#include <QPainter>
#include <QFont>
#include <QApplication>
#include <QSysInfo>
#include <QFile>

#include "SettingsController.h"

class Util : public QObject {
    Q_OBJECT    
public:
    explicit Util(QObject *parent = nullptr);
    static double calculateTemperature(const double &temperature, const QString &oldUnit, const QString &newUnit);
    static double calculateWindSpeed(double speed, const QString &oldUnit, const QString &newUnit);
    static double calculatePressure(double pressure, const QString &unit);
    static double kelvinToCelsiu(const double &temperature);
    static double fahrenheitToCelsiu(const double &temperature);
    static double celsiusToKelvin(const double &temperature);
    static double celsiusToFahrenheit(const double &temperature);
    static double kelvinToFahrenheit(const double &temperature);
    static double fahrenheitToKelvin(const double &temperature);
    static double msToMph(double speed);
    static double msToKph(double speed);
    static double mphToKph(double speed);
    static double mphToMs(double speed);
    static double kphToMph(double speed);
    static double kphToMs(double speed);
    static int roundToInt(const double &unit);
    static QString findFontCode(const QString &branch,const QString &code);

    Q_INVOKABLE static QString tempUnitSymbol(const QString &unit);
    Q_INVOKABLE static QString speedUnitSymbol(const QString &unit);
    Q_INVOKABLE static QString pressureUnitSymbol(const QString &unit);
    Q_INVOKABLE static QString osType();
    Q_INVOKABLE static QString firstLetterUp(const QString &string);
    Q_INVOKABLE static QList<QObject*> creditsList();
    Q_INVOKABLE static QString iconPathPrefix();
public slots:
    QString getLogoImage() const;

};

#endif // UTIL_H
