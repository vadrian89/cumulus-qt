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
#include "ThreadWorker.h"
#include "Util.h"
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QThread>
#include <QFont>
#include <QTimer>

ThreadWorker::ThreadWorker(QObject *parent) : QObject(parent) {
    connect(this, SIGNAL(startUpdateTimerSignal()), this, SLOT(updaterTimerStart()));
}

void ThreadWorker::updaterTimerStart() {
    QTimer::singleShot(60000, this, SLOT(startLookingForUpdates()));
}

void ThreadWorker::startLookingForUpdates() {
    qDebug() << "Updater started.";
    QString maintenancetoolPath = QCoreApplication::applicationDirPath() + "/maintenancetool";
    if (!QFile::exists(maintenancetoolPath)) {
        qDebug() << "maintenancetool is missing!";
        qDebug() << "For automatic updates download the online installer!";
        emit updateSearchFinished();
    }
    else {
        QProcess process;
        process.start(maintenancetoolPath + " --checkupdates");
        process.waitForFinished();

        if(process.error() != QProcess::UnknownError) {
            qDebug() << "Error checking for updates!";
            qDebug() << "Error message: " + process.errorString();
        }
        else {
            QByteArray data = process.readAllStandardOutput();
            qDebug() << "data: " << data;
            if (data.isEmpty()) {
                qDebug() << "No updates are available!";
                emit startUpdateTimerSignal();
            }
            else if (QString(data).toLower().contains(QRegExp("(error|null|empty)"))) {
                qDebug() << "Error occurred while searching for updates!";
                qDebug() << "Error: " << data;
                emit startUpdateTimerSignal();
            }
            else {
                QStringList args("--updater");
                QProcess::startDetached(maintenancetoolPath, args);
            }
            emit updateSearchFinished();
        }
    }
}

void ThreadWorker::createTrayIcon(const QString &weather, const QString &theme) {
    QString color = theme == "light" ? "white" : "black";
    QImage image(22, 22, QImage::Format_ARGB32_Premultiplied);
    QFont font;
    font.setPixelSize(14);
    font.setFamily("Arial");
    font.setBold(true);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    painter.setFont(font);
    painter.setPen(QColor(color));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawText(image.rect(), Qt::AlignCenter, weather);
    emit finishedCreatingPixmap(image);
    emit stopThread();
}
