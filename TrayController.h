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
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(bool startUp READ startUp WRITE setStartUp NOTIFY startUpChanged)
    Q_PROPERTY(bool trayVisibility READ trayVisibility WRITE setTrayVisibility NOTIFY trayVisibilityChanged)

    QString m_icon;
    bool m_startUp, m_trayVisibility;
    QSystemTrayIcon *trayIcon;

    void initialiseTray();
    void disableTray();
    void enableTray();
    void setTrayIcon(const QString &weather);
    QThread *thread;
    ThreadWorker *worker;
    QAction *currentWeatherAction;
public:
    explicit TrayController(QObject *parent = 0);
    QString icon() const;
    void setIcon(const QString &icon);
    bool startUp() const;
    void setStartUp(const bool &startUp);
    bool trayVisibility() const;
    void setTrayVisibility(const bool &trayVisibility);
private slots:
    void emitCloseApp();
    void emitShowGui();
    void setTrayIcon(const QPixmap &pixmap);
signals:
    void iconChanged();
    void startUpChanged();
    void trayVisibilityChanged();
    void closeApp();
    void showGui();
};

#endif // TRAYCONTROLLER_H
