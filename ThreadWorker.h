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

class ThreadWorker : public QObject {
    Q_OBJECT
public:
    explicit ThreadWorker(QObject *parent = 0);    
    void createWeatherPixmap(const QString &weather);
//    void changeActionData(const QAction* action, const );
public slots:
    void updaterTimerStart();
private slots:
    void startLookingForUpdates();
signals:
    void startUpdateTimerSignal();
    void updateSearchFinished();
    void finishedCreatingPixmap(QPixmap pixmap);
    void stopThread();
};

#endif // THREADWORKER_H
