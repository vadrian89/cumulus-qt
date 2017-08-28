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
#include <QProcessEnvironment>
#include "Util.h"
#include "CreditsAuthor.h"

Util::Util(QObject *parent) : QObject(parent){}

QString Util::getTemperatureUnit() {
    QSettings settings;
    settings.beginGroup("weather-settings");
    QString unit = settings.value("temperatureUnit", "c").toString();
    settings.endGroup();
    return unit.toUpper();
}

void Util::setTemperatureUnit(QString unit) {
    QSettings settings;
    settings.beginGroup("weather-settings");
    settings.setValue("temperatureUnit", unit);
    settings.endGroup();
}

QString Util::getWindSpeedUnit() {
    QSettings settings;
    settings.beginGroup("weather-settings");
    QString unit = settings.value("windSpeedUnit", "m/s").toString();
    settings.endGroup();
    return unit;
}

void Util::setWindSpeedUnit(QString unit) {
    QSettings settings;
    settings.beginGroup("weather-settings");
    settings.setValue("windSpeedUnit", unit);
    settings.endGroup();
}

QString Util::getPressureUnit() {
    QSettings settings;
    settings.beginGroup("weather-settings");
    QString unit = settings.value("pressureUnit", "mbar").toString();
    settings.endGroup();
    return unit;
}

QString Util::backgroundColor() {
    QSettings settings;
    settings.beginGroup("app-settings");
    QString color = settings.value("applicationBackground", "#ff0099ff").toString();
    settings.endGroup();
    return color;
}

QString Util::textColor() {
    QSettings settings;
    settings.beginGroup("app-settings");
    QString color = settings.value("textColor", "#ffffff").toString();
    settings.endGroup();
    return color;
}

QString Util::getWeatherApi() {
    QSettings settings;
    settings.beginGroup("weather-settings");
    QString api = settings.value("api", "owm").toString();
    settings.endGroup();
    return api;
}

void Util::setWeatherApi(const QString &weatherApi) {
    QSettings settings;
    settings.beginGroup("weather-settings");
    settings.setValue("api", weatherApi);
    settings.endGroup();
}

//Calculate temperature based on local selection of measurement
double Util::calculateTemperature(const double &temperature, const QString &unit) {
    QString localUnit = getTemperatureUnit().toLower();
    double finalValue = 0;
    if (localUnit == "c" && unit == "f") {
        finalValue = fahrenheitToCelsiu(temperature);
    }
    else if (localUnit == "c" && unit == "k") {
        finalValue = kelvinToCelsiu(temperature);
    }
    else if (localUnit == "k" && unit == "f") {
        finalValue = fahrenheitToKelvin(temperature);
    }
    else if (localUnit == "k" && unit == "c") {
        finalValue = celsiusToKelvin(temperature);
    }
    else if (localUnit == "f" && unit == "c") {
        finalValue = celsiusToFahrenheit(temperature);
    }
    else if (localUnit == "f" && unit == "k") {
        finalValue = kelvinToFahrenheit(temperature);
    }
    else {        
        finalValue = temperature;
    }
    return roundToInt(finalValue);
}

//Calculate speed based on local selection of speed unit
double Util::calculateWindSpeed(double speed, const QString &unit) {
    QString localUnit = getWindSpeedUnit().toLower();
    double finalSpeed = 0;
    if (localUnit == "m/s" && unit == "kph") {
        finalSpeed = kphToMs(speed);
    }
    else if (localUnit == "m/s" && unit == "mph") {
        finalSpeed = mphToMs(speed);
    }
    else if (localUnit == "kph" && unit == "m/s") {
        finalSpeed = msToKph(speed);
    }
    else if (localUnit == "kph" && unit == "mph") {
        finalSpeed = mphToKph(speed);
    }
    else if (localUnit == "mph" && unit == "m/s") {
        finalSpeed = msToMph(speed);
    }
    else if (localUnit == "mph" && unit == "kph") {
        finalSpeed = kphToMph(speed);
    }
    else {
        finalSpeed = speed;
    }
    return roundToInt(finalSpeed);
}

double Util::kelvinToCelsiu(const double &temperature) {
    return temperature - 273.15;
}

double Util::fahrenheitToCelsiu(const double &temperature) {
    return ((temperature - 32) / 1.8);
}

double Util::celsiusToKelvin(const double &temperature) {
    return temperature + 273.15;
}

double Util::celsiusToFahrenheit(const double &temperature) {
    return ((temperature * 1.8) + 32);
}

double Util::kelvinToFahrenheit(const double &temperature) {
    return celsiusToFahrenheit(kelvinToCelsiu(temperature));
}

double Util::fahrenheitToKelvin(const double &temperature) {
    return celsiusToKelvin(fahrenheitToCelsiu(temperature));
}

double Util::msToMph(double speed) {
    return speed / 0.44704;
}

double Util::msToKph(double speed) {
    return speed * 3.6;
}

double Util::mphToKph(double speed) {
    return speed * 1.609344;
}

double Util::mphToMs(double speed) {
    return kphToMs(mphToKph(speed));
}

double Util::kphToMph(double speed) {
    return msToMph(kphToMs(speed));
}

double Util::kphToMs(double speed) {
    return speed * 0.27;
}

double Util::calculatePressure(double pressure, const QString &unit) {
    QString str = getPressureUnit().toLower();
    double finalValue = pressure;

    if (str == "mbar" && unit == "mmhg") {
        finalValue = pressure * 0.75;
    }
    if (str == "mmhg" && unit == "mbar") {
        finalValue = pressure * 1.333;
    }

    return QString::number(finalValue, 'd', 1).toDouble();
}

QString Util::temperatureUnitSymbol() {
    QString str = getTemperatureUnit().toLower();
    if (str == "c") {
        return " °C";
    }
    else if (str == "k") {
        return " °K";
    }
    else {
        return " °F";
    }
}

QString Util::speedUnitSymbol() {
    QString str = getWindSpeedUnit().toLower();
    if (str == "mph") {
        return " MPH";
    }
    else if (str == "kph") {
        return " Km/h";
    }
    else {
        return " m/s";
    }
}

QString Util::pressureUnitSymbol() {
    QString str = getPressureUnit().toLower();
    if (str == "mbar") {
        return " mbar";
    }
    else if (str == "mmhg") {
        return " mmHg";
    }
    else {
        return str;
    }
}

QString Util::osType() {
    return QSysInfo::productType();
}

QString Util::firstLetterUp(const QString string) {
    QString firstLetter = string;
    firstLetter.remove(1, string.size());
    QString finalString = string;
    finalString.replace(0, 1, firstLetter.toUpper());
    return finalString;
}

int Util::roundToInt(const double &unit) {
    if (unit >= 0)
        return unit + 0.5;
    else
        return unit - 0.5;
}

QString Util::getLogoImage() {
    QString prefix = "file:icons/";
#if defined(Q_OS_ANDROID)
    prefix = "assets:/";
#endif
    if (getWeatherApi() == "y") {
        return prefix + "yw_logo.png";
    }
    else if (getWeatherApi() == "wund") {
        return prefix + "wund_logo_light.png";
    }
    else {
        return "image://customimage/Powered by:#OpenWeatherMap" + textColor();
    }
}

bool Util::trayVisibility() {
    QSettings settings;
    settings.beginGroup("app-settings");
    bool visible = settings.value("tray", false).toBool();
    settings.endGroup();
    return visible;
}

QList<QObject*> Util::creditsList() {
    QList<QObject*> list;
    CreditsAuthor *vectorsMarket = new CreditsAuthor(0, "Vectors Market", "http://www.flaticon.com/authors/vectors-market", "map.png", "CC 3.0 BY");
    CreditsAuthor *madebyoliver2 = new CreditsAuthor(0, "Madebyoliver", "http://www.flaticon.com/authors/madebyoliver", "speed_icon.png", "CC 3.0 BY");
    CreditsAuthor *pixelBuddha = new CreditsAuthor(0, "Pixel Buddha", "http://www.flaticon.com/authors/pixel-buddha", "temperature_icon.png", "CC 3.0 BY");
    CreditsAuthor *baianat = new CreditsAuthor(0, "Baianat", "http://www.flaticon.com/authors/baianat", "palette.png", "CC 3.0 BY");
    CreditsAuthor *popcornsArts = new CreditsAuthor(0, "Popcorns Arts", "http://www.flaticon.com/authors/popcorns-arts", "api.png", "CC 3.0 BY");
    CreditsAuthor *vectorsMarket2 = new CreditsAuthor(0, "Vectors Market", "https://www.flaticon.com/authors/vectors-market", "credits.png", "CC 3.0 BY");
    CreditsAuthor *freepikLogo = new CreditsAuthor(0, "Freepik", "http://www.freepik.com", "cumulus.png", "CC 3.0 BY");
    list.append(vectorsMarket);
    list.append(madebyoliver2);
    list.append(pixelBuddha);
    list.append(baianat);
    list.append(popcornsArts);
    list.append(vectorsMarket2);
    list.append(freepikLogo);
    return list;
}

QString Util::trayTheme() {
    QSettings settings;
    settings.beginGroup("app-settings");
    QString string = settings.value("trayTheme", "light").toString();
    settings.endGroup();
    return string;
}

QString Util::windowControlsPos() {
    QSettings settings;
    settings.beginGroup("app-settings");
    QString string = settings.value("windowControlsPos", "left").toString();
    settings.endGroup();
    return string;
}

#include <QApplication>
#include <QDebug>
QString Util::iconPathPrefix() {
    QProcessEnvironment pEnv = QProcessEnvironment::systemEnvironment();
    QString prefix = "file:icons/hicolor/512x512/";
    if (pEnv.contains("APPIMAGE"))
        prefix = "file:" + QApplication::applicationDirPath().remove("/usr/bin") + "/usr/share/pixmaps/";
#if defined(Q_OS_ANDROID)
    prefix = "assets:/";
#endif
    qDebug() << "Util::iconPathPrefix: " << prefix;
    return prefix;
}
