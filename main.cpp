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
#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include <QIcon>

#include "CppFiles/WeatherType.h"
#include "CppFiles/Forecast.h"
#include "CppFiles/Util.h"
#include "CppFiles/SettingsController.h"
#include "CppFiles/SearchLocation.h"
#include "CppFiles/TrayController.h"
#include "CppFiles/ThreadWorker.h"
#include "CppFiles/CustomImageProvider.h"

void registerQmlType();

int main(int argc, char *argv[]) {
    QString applicationName = "Cumulus";
    if (argc > 2 && QString::fromLatin1(argv[2]) == "-i") {
        applicationName = argv[3];
    }
    QCoreApplication::setOrganizationName("Visoft");
    QCoreApplication::setApplicationName(applicationName);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    Util util;    
    QThread thread;
    QQmlApplicationEngine engine;
    engine.addImageProvider(QLatin1String("customimage"), new CustomImageProvider());
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("util", &util);
    context->setContextProperty("applicationPath", "file://" + qApp->applicationDirPath() + "/");
    registerQmlType();
    if (Util::osType() == "android")
        engine.load(QUrl(QLatin1String("qrc:/main-android.qml")));
    else
        engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    ThreadWorker *threadWorker = new ThreadWorker();
    threadWorker->moveToThread(&thread);
    QObject::connect(&thread, SIGNAL(finished()), threadWorker, SLOT(deleteLater()));
    QObject::connect(threadWorker, SIGNAL(updateSearchFinished()), &thread, SLOT(quit()));
    thread.start();
    threadWorker->updaterTimerStart();

    return app.exec();    
}

void registerQmlType() {
    qmlRegisterType<WeatherType>("ownTypes.weather", 1, 8, "Weather");
    qmlRegisterType<SettingsController>("ownTypes.settingsController", 0, 6, "SettingsController");
    qmlRegisterType<SearchLocation>("ownTypes.searchLocation", 0, 4, "LocationSearchController");
    qmlRegisterType<TrayController>("ownTypes.TrayController", 0, 3, "TrayController");
}
