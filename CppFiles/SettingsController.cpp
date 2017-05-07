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
#include "SettingsController.h"

SettingsController::SettingsController(QObject *parent) : QObject(parent) {
    QSettings settings;
    settings.beginGroup("app-settings");
    m_trayVisibility = settings.value("tray", false).toBool();    
    settings.endGroup();
    settings.beginGroup("app-settings");
    m_trayTheme = settings.value("trayTheme", "light").toString();
    m_windowControlsPos = settings.value("windowControlsPos", "left").toString();
    settings.endGroup();
}

QString SettingsController::applicationBackground() const {
    QSettings settings;
    settings.beginGroup("app-settings");
    QString background = settings.value("applicationBackground", "#ff0099ff").toString();
    settings.endGroup();
    return background;
}

void SettingsController::setApplicationBackground(const QString &applicationBackground) {
    QSettings settings;
    settings.beginGroup("app-settings");
    settings.setValue("applicationBackground", applicationBackground);
    settings.endGroup();
    emit applicationBackgroundChanged();
}

QString SettingsController::textColor() const {
    QSettings settings;
    settings.beginGroup("app-settings");
    QString color = settings.value("textColor", "#ffffff").toString();
    settings.endGroup();
    return color;
}

void SettingsController::setTextColor(const QString &textColor) {
    QSettings settings;
    settings.beginGroup("app-settings");
    settings.setValue("textColor", textColor);
    settings.endGroup();
    emit textColorChanged();
}

float SettingsController::applicationOpacity() const {
    QSettings settings;
    settings.beginGroup("app-settings");
    float opacity = settings.value("applicationOpacity", "1.0").toFloat();
    settings.endGroup();
    return opacity;
}

void SettingsController::setApplicationOpacity(const float &applicationOpacity) {
    QSettings settings;
    settings.beginGroup("app-settings");
    settings.setValue("applicationOpacity", applicationOpacity);
    settings.endGroup();
    emit applicationOpacityChanged();
}

void SettingsController::setTrayVisibility(const bool &trayVisibility) {
    if (m_trayVisibility != trayVisibility) {
        QSettings settings;
        settings.beginGroup("app-settings");
        settings.setValue("tray", trayVisibility);
        settings.endGroup();
        m_trayVisibility = trayVisibility;
        emit trayVisibilityChanged();
    }
}

bool SettingsController::trayVisibility() const {
    return m_trayVisibility;
}

void SettingsController::setTrayTheme(const QString &trayTheme) {
    if (m_trayTheme != trayTheme) {
        QSettings settings;
        settings.beginGroup("app-settings");
        settings.setValue("trayTheme", trayTheme);
        settings.endGroup();
        m_trayTheme = trayTheme;
        emit trayThemeChanged();
    }
}

QString SettingsController::trayTheme() const {
    return m_trayTheme;
}

void SettingsController::setWindowControlsPos(const QString &windowControlsPos) {
    if (m_windowControlsPos != windowControlsPos) {
        QSettings settings;
        settings.beginGroup("app-settings");
        settings.setValue("windowControlsPos", windowControlsPos);
        settings.endGroup();
        m_windowControlsPos = windowControlsPos;
        emit windowControlsPosChanged();
    }
}

QString SettingsController::windowControlsPos() const {
    return m_windowControlsPos;
}
