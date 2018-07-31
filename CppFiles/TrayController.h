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
#ifndef TRAYCONTROLLER_H
#define TRAYCONTROLLER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QPixmap>
#include <QPainter>
#include <QThread>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QImageReader>
#include <QFile>
#include <QDebug>

#include "ThreadWorker.h"
class TrayController : public QObject {
    Q_OBJECT
    QString m_trayTheme;
    bool m_trayVisibility;
    QSystemTrayIcon *trayIcon;
    int m_temperature;
    Q_PROPERTY(int temperature MEMBER m_temperature WRITE setTemperature NOTIFY temperatureChanged)
    Q_PROPERTY(bool trayVisibility READ trayVisibility WRITE setTrayVisibility NOTIFY trayVisibilityChanged)
    Q_PROPERTY(QString trayTheme READ trayTheme WRITE setTrayTheme NOTIFY trayThemeChanged)

    void initialiseTray();
    void disableTray();
    void enableTray();
    QThread *thread;
    ThreadWorker *worker;
    QAction *currentWeatherAction;
    QImage createTrayIcon(const int &temperature, const QString &theme);
    bool isTrayAvailable();
public:
    explicit TrayController(QObject *parent = nullptr);
    bool startUp() const;
    void setStartUp(const bool &startUp);
    bool trayVisibility() const;
    void setTrayVisibility(const bool &trayVisibility);
    QString trayTheme() const;
    void setTrayTheme(const QString &trayTheme);
    void setTemperature(const int &temperature);
private slots:
    void emitCloseApp();
    void emitShowGui();
    void setTrayIcon();
signals:
    void trayVisibilityChanged();
    void closeApp();
    void showGui();
    void trayThemeChanged();
    void temperatureChanged();
};

#endif // TRAYCONTROLLER_H
