#ifndef THREADWORKER_H
#define THREADWORKER_H

#include <QObject>
#include <QProcess>
#include <QTimer>

class ThreadWorker : public QObject {
    Q_OBJECT
public:
    explicit ThreadWorker(QObject *parent = 0);    
public slots:
    void updaterTimerStart();
private slots:
    void startLookingForUpdates();
signals:
    void startUpdateTimerSignal();
    void updateSearchFinished();
};

#endif // THREADWORKER_H
