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

TrayController::TrayController(QObject *parent) :
    QObject(parent),
    trayIcon(nullptr) {}

void TrayController::setIcon(const QString &icon) {
    if (m_icon != icon && isTrayAvailable()) {
        m_icon = icon;
        setTrayIcon();
        emit iconChanged();
    }
}

QString TrayController::icon() const {
    return m_icon;
}

void TrayController::setTrayVisibility(const bool &trayVisibility) {
    if (m_trayVisibility != trayVisibility) {
        if (trayVisibility)
            enableTray();
        else
            disableTray();
        m_trayVisibility = trayVisibility;
        emit trayVisibilityChanged();
    }
}

bool TrayController::trayVisibility() const {
    return m_trayVisibility;
}

void TrayController::setTrayTheme(const QString &trayTheme) {
    if (m_trayTheme != trayTheme && isTrayAvailable()) {
        m_trayTheme = trayTheme;
        setTrayIcon();
        emit trayThemeChanged();
    }
}

QString TrayController::trayTheme() const {
    return m_trayTheme;
}

void TrayController::setTrayIcon() {
    if (isTrayAvailable()) {
        trayIcon->setIcon(QIcon(QPixmap::fromImage(createTrayIcon(m_icon + "°", m_trayTheme))));
    }
}

void TrayController::initialiseTray() {
    if (!isTrayAvailable()) {
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
        trayIcon->show();
    }
}

void TrayController::enableTray() {
    initialiseTray();
    if (isTrayAvailable())
        setTrayIcon();
}

void TrayController::disableTray() {
    if (isTrayAvailable()) {
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

QImage TrayController::createTrayIcon(const QString &weather, const QString &theme) {
    QString color = theme == "light" ? "white" : "black";
    QImage image(44, 44, QImage::Format_ARGB32_Premultiplied);
    QFont font;
    font.setPixelSize(26);
    font.setBold(true);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    painter.setFont(font);
    painter.setPen(QColor(color));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawText(image.rect(), Qt::AlignCenter, weather);
    return image;
}

bool TrayController::isTrayAvailable() {
    return (QSystemTrayIcon::isSystemTrayAvailable()
            && trayIcon != nullptr
            && trayIcon->isVisible());
}
