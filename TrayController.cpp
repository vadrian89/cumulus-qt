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
        connect(worker, SIGNAL(finishedCreatingPixmap(QPixmap)), this, SLOT(setTrayIcon(QPixmap)));
        connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
        worker->moveToThread(thread);
        thread->start();
        worker->createWeatherPixmap(weather + "°");
    }
}

void TrayController::setTrayIcon(const QPixmap &pixmap) {
    trayIcon->setIcon(QIcon(pixmap));
    thread->quit();
    thread->wait();
}

void TrayController::initialiseTray() {
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        QIcon firstIcon(":/data/cumulus.svg");
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
