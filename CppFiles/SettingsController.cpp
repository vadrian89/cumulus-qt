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
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QSysInfo>

SettingsController::SettingsController(QObject *parent) : QObject(parent) {
    QSettings settings;
    settings.beginGroup("app-settings");
    m_trayVisibility = settings.value("tray", false).toBool();            
    m_trayTheme = settings.value("trayTheme", "light").toString();
    m_windowControlsPos = settings.value("windowControlsPos", "left").toString();
    m_applicationBackground = settings.value("applicationBackground", "#ff0099ff").toString();
    m_textColor = settings.value("textColor", "#ffffff").toString();
    m_applicationOpacity = settings.value("applicationOpacity", "1.0").toFloat();
    m_loginStart = settings.value("loginStart", QFile::exists(QDir::homePath() + "/.config/autostart/cumulus.desktop")).toBool();
    settings.endGroup();
}

void SettingsController::setApplicationBackground(const QString &applicationBackground) {
    if (m_applicationBackground != applicationBackground) {
        QSettings settings;
        settings.beginGroup("app-settings");
        settings.setValue("applicationBackground", applicationBackground);
        settings.endGroup();
        m_applicationBackground = applicationBackground;
        emit applicationBackgroundChanged();
    }
}

void SettingsController::setTextColor(const QString &textColor) {
    if (m_textColor != textColor) {
        QSettings settings;
        settings.beginGroup("app-settings");
        settings.setValue("textColor", textColor);
        settings.endGroup();
        m_textColor = textColor;
        emit textColorChanged();
    }
}

void SettingsController::setApplicationOpacity(const float &applicationOpacity) {
    if (m_applicationOpacity != applicationOpacity) {
        QSettings settings;
        settings.beginGroup("app-settings");
        settings.setValue("applicationOpacity", applicationOpacity);
        settings.endGroup();
        m_applicationOpacity = applicationOpacity;
        emit applicationOpacityChanged();
    }
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

void SettingsController::setLoginStart(const bool &loginStart) {
    if (m_loginStart != loginStart) {
        m_loginStart = loginStart;
        if (QSysInfo::kernelType() == "linux" && QSysInfo::productType() != "android") {
            loginStartLinux(m_loginStart);
        }
        emit loginStartChanged();
    }
}


void SettingsController::loginStartLinux(const bool &loginStart) {
    QString appName = QApplication::applicationName();
    QString loginStartDirPath = QDir::homePath() + "/.config/autostart";
    QString loginStartFilePath = loginStartDirPath + "/cumulus.desktop";
    if (appName != "Cumulus") {
        loginStartFilePath = loginStartDirPath + "/cumulus-" + appName + ".desktop";
    }
    QDir dir(loginStartDirPath);
    QFile file(loginStartFilePath);
    if (loginStart == true) {
        if (dir.exists() == false) {
            dir.mkpath(dir.path());
        }
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "SettingsController::loginStartLinux: could not open file " + file.fileName();
            return;
        }

        QTextStream out(&file);
        out << "[Desktop Entry]" << endl;
        out << "Type=Application" << endl;
        out << "Terminal=false" << endl;
        out << "Categories=Utility;" << endl;
        if (appName != "Cumulus") {
            out << "Exec=" + qApp->applicationDirPath() + "/Cumulus -i " << appName << endl;
        }
        else {
            out << "Exec=" + qApp->applicationDirPath() + "/Cumulus" << endl;
        }
        out << "Name=Cumulus" << endl;
        out << "Icon=" + qApp->applicationDirPath() + "/cumulus.svg";
        out.flush();
        file.close();
        file.setPermissions(QFileDevice::ExeOwner | QFileDevice::ReadOwner | QFileDevice::WriteOwner);

    }
    else {
        if (!file.remove()) {
            qDebug() << "SettingsController::loginStartLinux: could not remove file " + file.fileName();
        }
    }
}

bool SettingsController::loginStartCheck() {    
    QString appName = QApplication::applicationName();
    QString fileName = QDir::homePath() + "/.config/autostart/cumulus.desktop";    
    if (appName != "Cumulus") {
        fileName = QDir::homePath() + "/.config/autostart/cumulus-" + appName + ".desktop";
    }
    QSettings settings;
    settings.beginGroup("app-settings");
    bool loginStart = settings.value("loginStart", QFile::exists(fileName)).toBool();
    settings.endGroup();
    return  loginStart;
}
