/*
* Copyright (C) 2016 Adrian Verban <verbanady@gmail.com>
* Maintainers: Adrian Verban, Archisman Panigrahi, Daryl Bennett
* Derived from Typhoon by Archisman Panigrahi which is based on Stormcloud by Jono Cooper <jonocooper.com>
* Thanks to all the contributors.
* Using the Ubuntu Condensed font.
* Thanks to Adam Whitcroft <adamwhitcroft.com> for Climacons!
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
#include "Utilities.h"
#include <QtDBus>

Utilities::Utilities(QObject *parent) : QObject(parent){}

void Utilities::saveWindowLocation(QPoint position) {
    QSettings settings;
    settings.beginGroup("window-settings");
    settings.setValue("pos", position);
    settings.endGroup();
}

QPoint Utilities::loadWindowLocation() {
    QSettings settings;
    settings.beginGroup("window-settings");
    QPoint pos = settings.value("pos", QPoint(0,0)).toPoint();
    settings.endGroup();
    return pos;
}

void Utilities::saveWindowSize(QSize size) {
    QSettings settings;
    settings.beginGroup("window-settings");
    settings.setValue("size", size);
    settings.endGroup();
}

QSize Utilities::loadWindowSize() {
    QSettings settings;
    settings.beginGroup("window-settings");
    QSize size = settings.value("size", QSize(300,500)).toSize();
    settings.endGroup();
    return size;
}

void Utilities::toggleLauncherCount(QString string) {
    QDBusMessage signal = QDBusMessage::createSignal("/", "com.canonical.Unity.LauncherEntry", "Update");
    signal << "application://cumulus.desktop";
    QVariantMap property;
    if (string.toLower() == "enable_launcher") {
        property.insert("count-visible", true);
    }
    else {
        property.insert("count-visible", false);
    }
    signal << property;
    QDBusConnection::sessionBus().send(signal);
}

void Utilities::setLauncherCount(int temperature) {
    QDBusMessage signal = QDBusMessage::createSignal("/", "com.canonical.Unity.LauncherEntry", "Update");
    signal << "application://cumulus.desktop";
    QVariantMap property;
    property.insert("count", qint64(temperature));
    signal << property;
    QDBusConnection::sessionBus().send(signal);
}