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
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDate>

Util::Util(QObject *parent) : QObject(parent){}

double Util::calculateTemperature(const double &temperature, const QString &oldUnit, const QString &newUnit) {
    double finalValue = 0;
    if (newUnit.toLower() == "c" && oldUnit.toLower() == "f") {
        finalValue = fahrenheitToCelsiu(temperature);
    }
    else if (newUnit.toLower() == "c" && oldUnit.toLower() == "k") {
        finalValue = kelvinToCelsiu(temperature);
    }
    else if (newUnit.toLower() == "k" && oldUnit.toLower() == "f") {
        finalValue = fahrenheitToKelvin(temperature);
    }
    else if (newUnit.toLower() == "k" && oldUnit.toLower() == "c") {
        finalValue = celsiusToKelvin(temperature);
    }
    else if (newUnit.toLower() == "f" && oldUnit.toLower() == "c") {
        finalValue = celsiusToFahrenheit(temperature);
    }
    else if (newUnit.toLower() == "f" && oldUnit.toLower() == "k") {
        finalValue = kelvinToFahrenheit(temperature);
    }
    else {
        finalValue = temperature;
    }
    return roundToInt(finalValue);
}

double Util::calculateWindSpeed(double speed, const QString &oldUnit, const QString &newUnit) {
    double finalSpeed = 0;
    if (newUnit.toLower() == "m/s" && oldUnit.toLower() == "kph") {        
        finalSpeed = kphToMs(speed);
    }
    else if (newUnit.toLower() == "m/s" && oldUnit.toLower() == "mph") {
        finalSpeed = mphToMs(speed);
    }
    else if (newUnit.toLower() == "kph" && oldUnit.toLower() == "m/s") {
        finalSpeed = msToKph(speed);
    }
    else if (newUnit.toLower() == "kph" && oldUnit.toLower() == "mph") {
        finalSpeed = mphToKph(speed);
    }
    else if (newUnit.toLower() == "mph" && oldUnit.toLower() == "m/s") {
        finalSpeed = msToMph(speed);
    }
    else if (newUnit.toLower() == "mph" && oldUnit.toLower() == "kph") {
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
    SettingsController settings;
    QString str = settings.pressureUnit().toLower();
    double finalValue = pressure;

    if (str == "mbar" && unit == "mmhg") {
        finalValue = pressure * 0.75;
    }
    if (str == "mmhg" && unit == "mbar") {
        finalValue = pressure * 1.333;
    }

    return QString::number(finalValue, 'd', 1).toDouble();
}

QString Util::tempUnitSymbol(const QString &unit) {
    QString str = unit.toLower();
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

QString Util::speedUnitSymbol(const QString &unit) {
    QString str = unit.toLower();
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

QString Util::pressureUnitSymbol(const QString &unit) {
    QString str = unit.toLower();
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

QString Util::firstLetterUp(const QString &string) {
    QString finalString = "";
    if (string != nullptr && string.trimmed().size() > 0) {
        QString firstLetter = string;
        firstLetter.remove(1, string.size());
        finalString = string;
        finalString.replace(0, 1, firstLetter.toUpper());
    }
    return finalString;
}

int Util::roundToInt(const double &unit) {
    if (unit >= 0.0)
        return static_cast<int>(unit + 0.5);
    else
        return static_cast<int>(unit - 0.5);
}

QString Util::getLogoImage() const {
    QString prefix = assetsPath() + "icons/";
    SettingsController settings;
    if (QProcessEnvironment::systemEnvironment().contains("APPIMAGE"))
        prefix = "file:" + QApplication::applicationDirPath().replace("/usr/bin", "/usr/share/icons/");
    else if (QSysInfo::productType() == "android")
        prefix = assetsPath();
    if (settings.weatherApi().toLower() == "y")
        return prefix + "yw_logo.png";
    else if (settings.weatherApi().toLower() == "wund")
        return prefix + "wund_logo_light.png";
    else
        return "image://customimage/Powered by:#OpenWeatherMap" + settings.textColor();
}

QList<QObject*> Util::creditsList() {
    QList<QObject*> list;
    CreditsAuthor *vectorsMarket = new CreditsAuthor(nullptr, "Vectors Market", "http://www.flaticon.com/authors/vectors-market", "map.png", "CC 3.0 BY");
    CreditsAuthor *madebyoliver2 = new CreditsAuthor(nullptr, "Madebyoliver", "http://www.flaticon.com/authors/madebyoliver", "speed_icon.png", "CC 3.0 BY");
    CreditsAuthor *pixelBuddha = new CreditsAuthor(nullptr, "Pixel Buddha", "http://www.flaticon.com/authors/pixel-buddha", "temperature_icon.png", "CC 3.0 BY");
    CreditsAuthor *baianat = new CreditsAuthor(nullptr, "Baianat", "http://www.flaticon.com/authors/baianat", "palette.png", "CC 3.0 BY");
    CreditsAuthor *popcornsArts = new CreditsAuthor(nullptr, "Popcorns Arts", "http://www.flaticon.com/authors/popcorns-arts", "api.png", "CC 3.0 BY");
    CreditsAuthor *vectorsMarket2 = new CreditsAuthor(nullptr, "Vectors Market", "https://www.flaticon.com/authors/vectors-market", "credits.png", "CC 3.0 BY");
    CreditsAuthor *freepikLogo = new CreditsAuthor(nullptr, "Freepik", "http://www.freepik.com", "cumulus.png", "CC 3.0 BY");
    list.append(vectorsMarket);
    list.append(madebyoliver2);
    list.append(pixelBuddha);
    list.append(baianat);
    list.append(popcornsArts);
    list.append(vectorsMarket2);
    list.append(freepikLogo);
    return list;
}

QString Util::iconPathPrefix() {
    QString prefix = assetsPath() + "icons/hicolor/512x512/";
    if (QProcessEnvironment::systemEnvironment().contains("APPIMAGE"))
        prefix = "file:" + QApplication::applicationDirPath().replace("/usr/bin", "/usr/share/icons/");
    else if (QSysInfo::productType() == "android")
        prefix = assetsPath();
    return prefix;
}

QString Util::findFontCode(const QString &branch,const QString &code) {
    QFile fontCodesFile(":/assets/weather_codes.json");
    QByteArray baJsonData;
    if (!fontCodesFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Util::findFontCode cannot open file " + fontCodesFile.fileName();
        return "";
    }
    while (!fontCodesFile.atEnd()) {
        baJsonData.append(fontCodesFile.readLine());
    }
    QJsonObject jsonObject = QJsonDocument::fromJson(baJsonData).object();
    QString result = jsonObject.find(branch).value().toObject().find(code).value().toString();
    return result;
}

QString Util::forecastDate(const QString &string) {
    QDate date = QDate::fromString(string, "dd/MMM/yyyy");
    return firstLetterUp(date.toString("dddd"));
}

QString Util::assetsPath() {
    if (QSysInfo::productType() == "android")
        return "assets:/";
    else {
        QString prefix = "file:" + QApplication::applicationDirPath() + "/assets/";
        if (QProcessEnvironment::systemEnvironment().contains("APPIMAGE"))
            prefix = "file:" + QApplication::applicationDirPath().replace("/usr/bin", "/usr/share/assets/");
        return prefix;
    }
}
