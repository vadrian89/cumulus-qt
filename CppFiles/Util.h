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

class Util : public QObject {
    Q_OBJECT    
public:
    explicit Util(QObject *parent = 0);
    static double calculateTemperature(const double &temperature, const QString &unit);
    static double calculateWindSpeed(double speed, const QString &unit);
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

    Q_INVOKABLE static QString getWeatherApi();
    Q_INVOKABLE static void setWeatherApi(const QString &weatherApi);
    Q_INVOKABLE static QString getTemperatureUnit();
    Q_INVOKABLE static void setTemperatureUnit(QString unit);
    Q_INVOKABLE static QString getWindSpeedUnit();
    Q_INVOKABLE static void setWindSpeedUnit(QString unit);
    Q_INVOKABLE static QString getPressureUnit();
    Q_INVOKABLE static void setPressureUnit(QString unit);
    Q_INVOKABLE static QString weatherIconsPath();
    Q_INVOKABLE static QString forecastIconsPath();
    Q_INVOKABLE static void setBackgroundColor(QString backgroundColor);
    Q_INVOKABLE static QString backgroundColor();
    Q_INVOKABLE static void setTextColor(QString textColor);
    Q_INVOKABLE static QString textColor();
    Q_INVOKABLE static qint32 getLocationId();
    Q_INVOKABLE static void setLocationId(const qint32 locationId);
    Q_INVOKABLE static QString windowButtonsBackground(bool mouseOver);
    Q_INVOKABLE static void saveWindowLocation(QPoint position);
    Q_INVOKABLE static QPoint loadWindowLocation();
    Q_INVOKABLE static void saveWindowSize(QSize size);
    Q_INVOKABLE static QSize loadWindowSize();
    Q_INVOKABLE static int indexFromApiCode(QString apiCode);
    Q_INVOKABLE static float applicationOpacity();
    Q_INVOKABLE static QString temperatureUnitSymbol();
    Q_INVOKABLE static QString speedUnitSymbol();
    Q_INVOKABLE static QString pressureUnitSymbol();
    Q_INVOKABLE static QString osType();
    Q_INVOKABLE static QString firstLetterUp(const QString string);
    Q_INVOKABLE static QString getLogoImage(const QString &text, const QString &color);
    Q_INVOKABLE static QString getLogoImage();
    Q_INVOKABLE static bool trayVisibility();
    Q_INVOKABLE static QList<QObject*> creditsList();
    Q_INVOKABLE static QString trayTheme();
    Q_INVOKABLE static QString windowControlsPos();
};

#endif // UTIL_H
