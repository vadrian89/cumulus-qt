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
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine>
#include <QSettings>
#include <QQmlContext>
#include <QThread>

#include "Utilities.h"
#include "ThreadWorker.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationName("Visoft");
    QCoreApplication::setApplicationName("Cumulus");
    QGuiApplication app(argc, argv);
    QtWebEngine::initialize();

    Utilities utilities;
    QThread thread;
    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("util", &utilities);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    ThreadWorker *threadWorker = new ThreadWorker();
    threadWorker->moveToThread(&thread);
    QObject::connect(&thread, SIGNAL(finished()), threadWorker, SLOT(deleteLater()));
    QObject::connect(threadWorker, SIGNAL(updateSearchFinished()), &thread, SLOT(quit()));
    thread.start();
    threadWorker->updaterTimerStart();

    return app.exec();
}
