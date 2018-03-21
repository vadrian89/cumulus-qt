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

api_keys SettingsController::API_KEYS;
QString SettingsController::WEATHER_SETTINGS_GROUP = "weather-settings";

SettingsController::SettingsController(QObject *parent) : QObject(parent) {
    QSettings settings;
    settings.beginGroup("app-settings");
    m_trayVisibility = settings.value("tray", false).toBool();            
    m_trayTheme = settings.value("trayTheme", "light").toString();
    m_windowControlsPos = settings.value("windowControlsPos", "left").toString();
    m_applicationBackground = settings.value("applicationBackground", "#ff0099ff").toString();
    m_textColor = settings.value("textColor", "#ffffff").toString();
    m_applicationOpacity = settings.value("applicationOpacity", "1.0").toFloat();
    m_loginStart = loginStartCheck();
    m_windowX = settings.value("windowX", 0).toInt();
    m_windowY = settings.value("windowY", 0).toInt();
    m_windowHeight = settings.value("windowHeight", 500).toInt();
    m_windowWidth = settings.value("windowWidth", 300).toInt();    
    settings.endGroup();
    settings.beginGroup(WEATHER_SETTINGS_GROUP);
    m_currentLocationId = settings.value("currentLocationId", -1).toInt();
    m_weatherApi = settings.value("api", "owm").toString();
    m_windSpeedUnit = settings.value("windSpeedUnit", "m/s").toString();
    m_tempUnit = settings.value("temperatureUnit", "c").toString();
    m_pressureUnit = settings.value("pressureUnit", "mbar").toString();
    m_useGps = settings.value("useGps", true).toBool();
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

QString SettingsController::textColor() const {
    return m_textColor;
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
            loginStartLinux(loginStart);
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
            out << "Exec=" + qApp->applicationDirPath() + "/Cumulus -i " << appName.remove("Cumulus") << endl;
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
    QString filePath = QDir::homePath() + "/.config/autostart/cumulus.desktop";
    if (appName != "Cumulus") {
        filePath = QDir::homePath() + "/.config/autostart/cumulus-" + appName + ".desktop";
    }    
    return QFile::exists(filePath);
}

void SettingsController::setWindowX(const int &windowX) {
    if (m_windowX != windowX) {
        m_windowX = windowX;
        QSettings settings;
        settings.beginGroup("app-settings");
        settings.setValue("windowX", windowX);
        settings.endGroup();
        emit windowXChanged();
    }
}

void SettingsController::setWindowY(const int &windowY) {
    if (m_windowY != windowY) {
        m_windowY = windowY;
        QSettings settings;
        settings.beginGroup("app-settings");
        settings.setValue("windowY", windowY);
        settings.endGroup();
        emit windowXChanged();
    }
}

void SettingsController::setWindowHeight(const int &windowHeight) {
    if (m_windowHeight != windowHeight) {
        m_windowHeight = windowHeight;
        QSettings settings;
        settings.beginGroup("app-settings");
        settings.setValue("windowHeight", windowHeight);
        settings.endGroup();
        emit windowXChanged();
    }
}

void SettingsController::setWindowWidth(const int &windowWidth) {
    if (m_windowWidth != windowWidth) {
        m_windowWidth = windowWidth;
        QSettings settings;
        settings.beginGroup("app-settings");
        settings.setValue("windowWidth", windowWidth);
        settings.endGroup();
        emit windowXChanged();
    }
}

void SettingsController::setCurrentLocationId(const int &locationId) {
    if (m_currentLocationId != locationId) {
        m_currentLocationId = locationId;
        QSettings settings;
        settings.beginGroup(WEATHER_SETTINGS_GROUP);
        settings.setValue("currentLocationId", m_currentLocationId);
        settings.endGroup();
        emit currentLocationIdChanged();
    }
}

int SettingsController::currentLocationId() const {
    return m_currentLocationId;
}

void SettingsController::setWeatherApi(const QString &weatherApi) {
    if (m_weatherApi != weatherApi && clearLocationCode()) {
        m_weatherApi = weatherApi;
        QSettings settings;
        settings.beginGroup(WEATHER_SETTINGS_GROUP);
        settings.setValue("api", m_weatherApi);
        settings.endGroup();
        emit weatherApiChanged();
    }
}

QString SettingsController::weatherApi() const {
    return m_weatherApi;
}

bool SettingsController::clearLocationCode() {
    DatabaseHelper dbHelper;
    return dbHelper.clearLocationCode();
}

void SettingsController::setWindSpeedUnit(const QString &windSpeedUnit) {
    if (m_windSpeedUnit != windSpeedUnit) {
        m_windSpeedUnit = windSpeedUnit;
        QSettings settings;
        settings.beginGroup(WEATHER_SETTINGS_GROUP);
        settings.setValue("windSpeedUnit", windSpeedUnit);
        settings.endGroup();
        emit windSpeedUnitChanged();
    }
}

QString SettingsController::windSpeedUnit() const {
    return m_windSpeedUnit;
}

void SettingsController::setTempUnit(const QString &tempUnit) {
    if (m_tempUnit != tempUnit) {
        m_tempUnit = tempUnit;
        QSettings settings;
        settings.beginGroup(WEATHER_SETTINGS_GROUP);
        settings.setValue("temperatureUnit", tempUnit);
        settings.endGroup();
        emit tempUnitChanged();
    }
}

QString SettingsController::tempUnit() const {
    return m_tempUnit;
}

void SettingsController::setPressureUnit(const QString &pressureUnit) {
    if (m_pressureUnit != pressureUnit) {
        m_pressureUnit = pressureUnit;
        QSettings settings;
        settings.beginGroup(WEATHER_SETTINGS_GROUP);
        settings.setValue("pressureUnit", pressureUnit);
        settings.endGroup();
        emit pressureUnitChanged();
    }
}

QString SettingsController::pressureUnit() const {
    return m_pressureUnit;
}

QString SettingsController::getWeatherApi() {
    QSettings settings;
    settings.beginGroup(WEATHER_SETTINGS_GROUP);
    QString api = settings.value("api", "owm").toString();
    settings.endGroup();
    return api;
}

bool SettingsController::useGps() const {
    return m_useGps;
}

void SettingsController::setUseGps(const bool &useGps) {
    if (m_useGps != useGps) {
        m_useGps = useGps;
        QSettings settings;
        settings.beginGroup(WEATHER_SETTINGS_GROUP);
        settings.setValue("useGps", useGps);
        settings.endGroup();
        emit useGpsChanged();
    }
}

void SettingsController::setApiKey(const QString &apiKey) {
    if (apiKey != this->apiKey()) {
        saveString(("apiKey" + this->weatherApi()), apiKey, WEATHER_SETTINGS_GROUP);
        emit apiKeyChanged();
    }
}

QString SettingsController::apiKey() const {
    QString fileLocation = QApplication::applicationDirPath() + "/api-keys.json";
    QFile apiKeysFile(fileLocation);
    QByteArray baJsonData;
    QString api = "";
    if (apiKeysFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!apiKeysFile.atEnd()) {
            baJsonData.append(apiKeysFile.readLine());
        }
        QJsonObject jsonObject = QJsonDocument::fromJson(baJsonData).object();
        api = jsonObject.find(getWeatherApi()).value().toString();
    }
    else {
        api = loadString(("apiKey" + this->weatherApi()), WEATHER_SETTINGS_GROUP);
        if (api.trimmed().size() == 0) {
            if (this->weatherApi() == "wund") {
                api = API_KEYS.wund;
            }
            else if (this->weatherApi() == "owm") {
                api = API_KEYS.owm;
            }
        }
    }
    return api;
}

void SettingsController::saveString(const QString &key, const QString &value, const QString &group) {
    QSettings settings;
    settings.beginGroup(group);
    settings.setValue(key, value);
    settings.endGroup();
}

QString SettingsController::loadString(const QString &key, const QString &group) const {
    QSettings settings;
    settings.beginGroup(group);
    QString value = settings.value(key, "").toString();
    settings.endGroup();
    return value;
}
