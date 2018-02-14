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
#include "Util.h"

TrayController::TrayController(QObject *parent) : QObject(parent) {
    trayIcon = nullptr;
    m_trayVisibility = false;
}

QString TrayController::icon() const {
    return m_icon;
}

void TrayController::setIcon(const QString &icon) {
    if (m_icon != icon && icon.trimmed().size() > 0) {
        m_icon = icon;
        setTrayIcon();
        emit iconChanged();
    }
}

bool TrayController::trayVisibility() const {
    return m_trayVisibility;
}

void TrayController::setTrayVisibility(const bool &trayVisibility) {
    if (m_trayVisibility != trayVisibility) {        
        if (trayVisibility == true) {
            if (trayIcon == nullptr)
                initialiseTray();
            enableTray();
        }
        else {
            if (trayIcon != nullptr)
                disableTray();
        }
        m_trayVisibility = trayVisibility;
        emit trayVisibilityChanged();
    }
}

void TrayController::setTrayIcon() {
    if (QSystemTrayIcon::isSystemTrayAvailable() && trayIcon != nullptr) {
        worker = new ThreadWorker();
        thread = new QThread(this);
        connect(worker, SIGNAL(finishedCreatingPixmap(QImage)), this, SLOT(setTrayIcon(QImage)));
        connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
        worker->moveToThread(thread);
        thread->start();
        worker->createTrayIcon(m_icon + "°", m_trayTheme);
    }
}

void TrayController::setTrayIcon(const QImage &image) {
    if (trayIcon != nullptr)
        trayIcon->setIcon(QIcon(QPixmap::fromImage(image)));
    thread->quit();
    thread->wait();
}

void TrayController::initialiseTray() {
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        QIcon firstIcon(qApp->applicationDirPath() +"/icons/hicolor/512x512/cumulus.png");
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
        setTrayIcon();
    }
}

void TrayController::disableTray() {
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        trayIcon->hide();
        delete trayIcon;
        trayIcon = nullptr;
    }
}

void TrayController::emitCloseApp() {
    emit closeApp();
}

void TrayController::emitShowGui() {
    emit showGui();
}

QString TrayController::trayTheme() const {
    return m_trayTheme;
}

void TrayController::setTrayTheme(const QString &trayTheme) {
    if (m_trayTheme != trayTheme && trayTheme.trimmed().size() > 0) {
        m_trayTheme = trayTheme;
        setTrayIcon();
        emit trayThemeChanged();
    }
}
