#include "TrayController.h"
#include "ThreadWorker.h"

TrayController::TrayController(QObject *parent) : QObject(parent) {
}

QString TrayController::icon() const {
    return m_icon;
}

void TrayController::setIcon(const QString &icon) {
    if (m_icon != icon) {
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

void TrayController::setTrayIcon(const QString &weather) {
    worker = new ThreadWorker();
    thread = new QThread(this);
    connect(worker, SIGNAL(finishedCreatingPixmap(QPixmap)), this, SLOT(setTrayIcon(QPixmap)));
    connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    worker->moveToThread(thread);
    thread->start();
    worker->createWeatherPixmap(weather + "°");
}

void TrayController::setTrayIcon(const QPixmap &pixmap) {
    trayIcon->setIcon(QIcon(pixmap));
    thread->quit();
    thread->wait();
}

void TrayController::initialiseTray() {
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        QIcon firstIcon(":/data/cumulus.svg");
//        qDebug() << QApplication::applicationDirPath() + "/cumulus.svg";
//        qDebug() << "exista " << QFile::exists(":/data/cumulus.svg");
        trayIcon = new QSystemTrayIcon();
        trayIcon->setIcon(firstIcon);
        QMenu *trayMenu = new QMenu();
        QAction *showAction = new QAction("Show", this);
        connect(showAction, SIGNAL(triggered(bool)), this, SLOT(emitShowGui()));
        QAction *closeAction = new QAction("Close", this);
        connect(closeAction, SIGNAL(triggered(bool)), this, SLOT(emitCloseApp()));
        trayMenu->addAction(showAction);
        trayMenu->addAction(closeAction);
        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();
    }
}

void TrayController::emitCloseApp() {
    emit closeApp();
}

void TrayController::emitShowGui() {
    emit showGui();
}
