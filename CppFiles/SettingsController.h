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
#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>
#include <QSettings>
#include <QPoint>
#include <QSize>
#include <memory>
#include <QDir>
#include <QFile>
#include <QApplication>
#include <QSysInfo>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "DatabaseHelper.h"

using namespace std;

struct api_keys {
    const QString wund = "30be6723cf95f92c";
    const QString owm = "15c2836a71c126524d35af1d29d563c5";
};

class SettingsController : public QObject {
    Q_OBJECT
    bool m_trayVisibility, m_loginStart, m_useGps;
    QString m_trayTheme, m_windowControlsPos, m_applicationBackground, m_textColor, m_weatherApi,
    m_windSpeedUnit, m_tempUnit, m_pressureUnit;
    float m_applicationOpacity;
    int m_windowX, m_windowY, m_windowHeight, m_windowWidth, m_currentLocationId;
    static api_keys API_KEYS;
    static QString WEATHER_SETTINGS_GROUP;

    Q_PROPERTY(QString applicationBackground MEMBER m_applicationBackground WRITE setApplicationBackground NOTIFY applicationBackgroundChanged)
    Q_PROPERTY(QString textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(float applicationOpacity MEMBER m_applicationOpacity WRITE setApplicationOpacity NOTIFY applicationOpacityChanged)
    Q_PROPERTY(bool trayVisibility READ trayVisibility WRITE setTrayVisibility NOTIFY trayVisibilityChanged)
    Q_PROPERTY(QString trayTheme MEMBER m_trayTheme WRITE setTrayTheme NOTIFY trayThemeChanged)
    Q_PROPERTY(QString windowControlsPos MEMBER m_windowControlsPos WRITE setWindowControlsPos NOTIFY windowControlsPosChanged)
    Q_PROPERTY(bool loginStart MEMBER m_loginStart WRITE setLoginStart NOTIFY loginStartChanged)
    Q_PROPERTY(int windowX MEMBER m_windowX WRITE setWindowX NOTIFY windowXChanged)
    Q_PROPERTY(int windowY MEMBER m_windowY WRITE setWindowY NOTIFY windowYChanged)
    Q_PROPERTY(int windowHeight MEMBER m_windowHeight WRITE setWindowHeight NOTIFY windowHeightChanged)
    Q_PROPERTY(int windowWidth MEMBER m_windowWidth WRITE setWindowWidth NOTIFY windowWidthChanged)
    Q_PROPERTY(int currentLocationId READ currentLocationId WRITE setCurrentLocationId NOTIFY currentLocationIdChanged)
    Q_PROPERTY(QString weatherApi READ weatherApi WRITE setWeatherApi NOTIFY weatherApiChanged)
    Q_PROPERTY(QString windSpeedUnit READ windSpeedUnit WRITE setWindSpeedUnit NOTIFY windSpeedUnitChanged)
    Q_PROPERTY(QString tempUnit READ tempUnit WRITE setTempUnit NOTIFY tempUnitChanged)
    Q_PROPERTY(QString pressureUnit READ pressureUnit WRITE setPressureUnit NOTIFY pressureUnitChanged)
    Q_PROPERTY(bool useGps READ useGps WRITE setUseGps NOTIFY useGpsChanged)
    Q_PROPERTY(QString apiKey READ apiKey WRITE setApiKey NOTIFY apiKeyChanged)

    void loginStartLinux(const bool &loginStart);
    bool clearLocationCode();
    void saveString(const QString &key, const QString &value, const QString &group);
    QString loadString(const QString &key, const QString &group) const;
public:
    explicit SettingsController(QObject *parent = nullptr);
    void setApplicationBackground(const QString &applicationBackground);
    void setTextColor(const QString &textColor);
    QString textColor() const;
    void setApplicationOpacity(const float &applicationOpacity);
    void setTrayVisibility(const bool &trayVisibility);
    bool trayVisibility() const;
    void setTrayTheme(const QString &trayTheme);
    void setWindowControlsPos(const QString &windowControlsPos);
    void setLoginStart(const bool &loginStart);
    void setWindowX(const int &windowX);
    void setWindowY(const int &windowY);
    void setWindowHeight(const int &windowHeight);
    void setWindowWidth(const int &windowWidth);
    void setCurrentLocationId(const int &locationId);
    int currentLocationId() const;
    void setWeatherApi(const QString &weatherApi);
    QString weatherApi() const;    
    void setWindSpeedUnit(const QString &windSpeedUnit);
    QString windSpeedUnit() const;
    void setTempUnit(const QString &tempUnit);
    QString tempUnit() const;
    void setPressureUnit(const QString &pressureUnit);
    QString pressureUnit() const;
    bool useGps() const;
    void setUseGps(const bool &useGps);
    void setApiKey(const QString &apiKey);
    QString apiKey() const;
    Q_INVOKABLE static bool loginStartCheck();
    Q_INVOKABLE static QString getWeatherApi();
signals:
    void applicationBackgroundChanged();
    void textColorChanged();
    void applicationOpacityChanged();
    void trayVisibilityChanged();
    void trayThemeChanged();
    void windowControlsPosChanged();
    void loginStartChanged();
    void windowXChanged();
    void windowYChanged();
    void windowHeightChanged();
    void windowWidthChanged();
    void currentLocationIdChanged();
    void weatherApiChanged();
    void tempUnitChanged();
    void windSpeedUnitChanged();
    void pressureUnitChanged();
    void useGpsChanged();
    void apiKeyChanged();
};

#endif // SETTINGSCONTROLLER_H
