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

    QString m_icon;
    bool m_startUp;
    QSystemTrayIcon *trayIcon;

    void initialiseTray();
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
private slots:
    void emitCloseApp();
    void emitShowGui();
    void setTrayIcon(const QPixmap &pixmap);
signals:
    void iconChanged();
    void startUpChanged();
    void closeApp();
    void showGui();
};

#endif // TRAYCONTROLLER_H
