/*
* Copyright (C) 2018 Adrian Verban <verbanady@gmail.com>
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
#include "MainWindow.h"
#include "WeatherType.h"
#include "Forecast.h"
#include "Util.h"
#include "SettingsController.h"
#include "TrayController.h"
#include "CustomImageProvider.h"
#include "FontImageProvider.h"
#include "Location.h"

#include <QSettings>
#include <QQmlEngine>
#include <QQmlContext>
#include <QApplication>
#include <QQuickWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_closeApp = false;
    QQuickWidget *view = new QQuickWidget();
    QQmlEngine *engine = view->engine();
    connect(engine, SIGNAL(quit()), this, SLOT(closeApp()));
    QQmlContext *context = engine->rootContext();
    Util *util = new Util(view);
    SettingsController *settingsController = new SettingsController(view);
    engine->addImageProvider(QLatin1String("customimage"), new CustomImageProvider());
    engine->addImageProvider(QLatin1String("fontimage"), new FontImageProvider());
    context->setContextProperty("util", util);
    context->setContextProperty("settingsController", settingsController);
    context->setContextProperty("applicationPath", "file://" + qApp->applicationDirPath() + "/");
    context->setContextProperty("applicationWindow", this);
    registerQmlType();
    view->setSource(QUrl(QLatin1String("qrc:/main-desktop.qml")));
    view->show();
    view->setResizeMode(QQuickWidget::SizeRootObjectToView);
    view->setClearColor(Qt::transparent);
    setAttribute(Qt::WA_TranslucentBackground);
    loadSettings();
    setCentralWidget(view);
    setMinimumHeight(150);
    setMinimumWidth(200);
    setWindowIcon(QIcon(Util::iconPathPrefix() + "cumulus.png"));
}

MainWindow::~MainWindow() {}

void MainWindow::loadSettings(){
    QSettings settings;
    settings.beginGroup("app-settings");
    if (!restoreGeometry(settings.value("geo").toByteArray()) && Util::osType() != "android")
        resize(300, 500);
    restoreState(settings.value("state").toByteArray());
    settings.endGroup();
}

void MainWindow::saveSettings(){
    QSettings settings;
    settings.beginGroup("app-settings");
    settings.setValue("geo",saveGeometry());
    settings.setValue("state",saveState());
    settings.endGroup();
}

void MainWindow::registerQmlType() {
    qmlRegisterType<WeatherType>("ownTypes.weather", 1, 9, "Weather");
    qmlRegisterType<SettingsController>("ownTypes.settingsController", 1, 0, "SettingsController");    
    qmlRegisterType<TrayController>("ownTypes.TrayController", 0, 3, "TrayController");
    qmlRegisterType<Location>("ownTypes.LocationController", 0, 1, "LocationController");
}

void MainWindow::closeEvent(QCloseEvent *event) {
    saveSettings();
    SettingsController settings;
    if (!m_closeApp && settings.trayVisibility()) {
        this->hide();
        event->ignore();
    }
    else {
        QMainWindow::closeEvent(event);
    }
}

void MainWindow::launchApp() {
    SettingsController settings;
    if (!settings.trayVisibility())
        this->show();
}

void MainWindow::closeApp() {
    m_closeApp = true;
    this->close();
}
