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
#include "TrayController.h"
#include "ThreadWorker.h"

TrayController::TrayController(QObject *parent) : QObject(parent) {
}

QString TrayController::icon() const {
    return m_icon;
}

void TrayController::setIcon(const QString &icon) {
    if (m_icon != icon && icon.trimmed().size() > 0) {
        setTrayIcon(icon);
        emit iconChanged();
    }
}

bool TrayController::startUp() const {
    return m_startUp;
}

void TrayController::setStartUp(const bool &startUp) {
    if (m_startUp != startUp) {
        if (startUp == true) {
            initialiseTray();
        }
        emit startUpChanged();
    }
}

bool TrayController::trayVisibility() const {
    return m_trayVisibility;
}

void TrayController::setTrayVisibility(const bool &trayVisibility) {
    if (m_trayVisibility != trayVisibility) {
        if (trayVisibility == true) {
            enableTray();
        }
        else {
            disableTray();
        }
        emit trayVisibilityChanged();
    }
}

void TrayController::setTrayIcon(const QString &weather) {
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        worker = new ThreadWorker();
        thread = new QThread(this);
        connect(worker, SIGNAL(finishedCreatingPixmap(QImage)), this, SLOT(setTrayIcon(QImage)));
        connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
        worker->moveToThread(thread);
        thread->start();
        worker->createWeatherPixmap(weather + "°");
    }
}

void TrayController::setTrayIcon(const QImage &image) {
    trayIcon->setIcon(QIcon(QPixmap::fromImage(image)));
    thread->quit();
    thread->wait();
}

void TrayController::initialiseTray() {
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        QIcon firstIcon(":/icons/icon.png");
        trayIcon = new QSystemTrayIcon();
        trayIcon->setIcon(firstIcon);
        QMenu *trayMenu = new QMenu();
        QAction *showAction = new QAction("Show", trayMenu);
        connect(showAction, SIGNAL(triggered(bool)), this, SLOT(emitShowGui()));
        QAction *closeAction = new QAction("Close", trayMenu);
        connect(closeAction, SIGNAL(triggered(bool)), this, SLOT(emitCloseApp()));
        trayMenu->addAction(showAction);
        trayMenu->addAction(closeAction);
        trayIcon->setContextMenu(trayMenu);
    }
}

void TrayController::enableTray() {
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        trayIcon->show();
    }
}

void TrayController::disableTray() {
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        trayIcon->hide();
    }
}

void TrayController::emitCloseApp() {
    emit closeApp();
}

void TrayController::emitShowGui() {
    emit showGui();
}
