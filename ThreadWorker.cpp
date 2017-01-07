#include "ThreadWorker.h"
#include <QDebug>
#include <QCoreApplication>
#include <QFile>

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
