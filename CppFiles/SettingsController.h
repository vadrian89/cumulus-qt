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

class SettingsController : public QObject {
    Q_OBJECT
    bool m_trayVisibility, m_loginStart;
    QString m_trayTheme, m_windowControlsPos, m_applicationBackground, m_textColor;
    float m_applicationOpacity;
    Q_PROPERTY(QString applicationBackground MEMBER m_applicationBackground WRITE setApplicationBackground NOTIFY applicationBackgroundChanged)
    Q_PROPERTY(QString textColor MEMBER m_textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(float applicationOpacity MEMBER m_applicationOpacity WRITE setApplicationOpacity NOTIFY applicationOpacityChanged)
    Q_PROPERTY(bool trayVisibility MEMBER m_trayVisibility WRITE setTrayVisibility NOTIFY trayVisibilityChanged)
    Q_PROPERTY(QString trayTheme MEMBER m_trayTheme WRITE setTrayTheme NOTIFY trayThemeChanged)
    Q_PROPERTY(QString windowControlsPos MEMBER m_windowControlsPos WRITE setWindowControlsPos NOTIFY windowControlsPosChanged)
    Q_PROPERTY(bool loginStart MEMBER m_loginStart WRITE setLoginStart NOTIFY loginStartChanged)

    void loginStartLinux(const bool &loginStart);
public:
    explicit SettingsController(QObject *parent = 0);
    void setApplicationBackground(const QString &applicationBackground);
    void setTextColor(const QString &textColor);
    void setApplicationOpacity(const float &applicationOpacity);
    void setTrayVisibility(const bool &trayVisibility);
    void setTrayTheme(const QString &trayTheme);
    void setWindowControlsPos(const QString &windowControlsPos);
    void setLoginStart(const bool &loginStart);

    Q_INVOKABLE static bool loginStartCheck();
signals:
    void applicationBackgroundChanged();
    void textColorChanged();
    void applicationOpacityChanged();
    void trayVisibilityChanged();
    void trayThemeChanged();
    void windowControlsPosChanged();
    void loginStartChanged();
};

#endif // SETTINGSCONTROLLER_H
