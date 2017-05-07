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
#ifndef THREADWORKER_H
#define THREADWORKER_H

#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QFontDatabase>
#include <QFont>
#include <QAction>
#include <QImage>

class ThreadWorker : public QObject {
    Q_OBJECT
public:
    explicit ThreadWorker(QObject *parent = 0);    
    void createWeatherPixmap(const QString &weather);
    void createTrayIcon(const QString &weather, const QString &theme);
//    void changeActionData(const QAction* action, const );
public slots:
    void updaterTimerStart();
private slots:
    void startLookingForUpdates();
signals:
    void startUpdateTimerSignal();
    void updateSearchFinished();
    void finishedCreatingPixmap(QImage image);
    void stopThread();
};

#endif // THREADWORKER_H
