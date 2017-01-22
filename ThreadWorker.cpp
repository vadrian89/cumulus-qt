#include "ThreadWorker.h"
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QThread>

ThreadWorker::ThreadWorker(QObject *parent) : QObject(parent) {
    connect(this, SIGNAL(startUpdateTimerSignal()), this, SLOT(updaterTimerStart()));
}

void ThreadWorker::updaterTimerStart() {
    QTimer::singleShot(60000, this, SLOT(startLookingForUpdates()));
}

void ThreadWorker::startLookingForUpdates() {
    qDebug() << "Updater started";
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
            emit startUpdateTimerSignal();
        }
        else {
            QByteArray data = process.readAllStandardOutput();
            if(data.isEmpty()) {
                qDebug() << "No updates are available!";
            }
            else {
                QStringList args("--updater");
                QProcess::startDetached(maintenancetoolPath, args);
            }
            emit updateSearchFinished();
        }
    }
}

void ThreadWorker::createWeatherPixmap(const QString &weather) {
//    int id = QFontDatabase::addApplicationFont(":/data/fonts/weathericons-regular-webfont.ttf");
//    QString iconsFamily = QFontDatabase::applicationFontFamilies(id).at(0);
//    QFont iconsFont(iconsFamily);
    QPixmap pixmap(22,22);
    pixmap.fill(QColor(0,0,0,0));
    QPainter painter(&pixmap);
    QThread::msleep(500);
    painter.setPen(QColor(Qt::white));
    painter.drawText(QRect(0, 0, 22, 22), Qt::AlignCenter, weather);
    emit finishedCreatingPixmap(pixmap);
    emit stopThread();
}
