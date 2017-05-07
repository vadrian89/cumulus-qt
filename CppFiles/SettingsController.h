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
    bool m_trayVisibility;
    QString m_trayTheme, m_windowControlsPos;
    Q_PROPERTY(QString applicationBackground READ applicationBackground WRITE setApplicationBackground NOTIFY applicationBackgroundChanged)
    Q_PROPERTY(QString textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(float applicationOpacity READ applicationOpacity WRITE setApplicationOpacity NOTIFY applicationOpacityChanged)
    Q_PROPERTY(bool trayVisibility READ trayVisibility WRITE setTrayVisibility NOTIFY trayVisibilityChanged)
    Q_PROPERTY(QString trayTheme READ trayTheme WRITE setTrayTheme NOTIFY trayThemeChanged)
    Q_PROPERTY(QString windowControlsPos READ windowControlsPos WRITE setWindowControlsPos NOTIFY windowControlsPosChanged)
public:
    explicit SettingsController(QObject *parent = 0);
    QString applicationBackground() const;
    void setApplicationBackground(const QString &applicationBackground);
    QString textColor() const;
    void setTextColor(const QString &textColor);
    float applicationOpacity() const;
    void setApplicationOpacity(const float &applicationOpacity);
    bool trayVisibility() const;
    void setTrayVisibility(const bool &trayVisibility);
    QString trayTheme() const;
    void setTrayTheme(const QString &trayTheme);
    QString windowControlsPos() const;
    void setWindowControlsPos(const QString &windowControlsPos);
signals:
    void applicationBackgroundChanged();
    void textColorChanged();
    void applicationOpacityChanged();
    void trayVisibilityChanged();
    void trayThemeChanged();
    void windowControlsPosChanged();
};

#endif // SETTINGSCONTROLLER_H
