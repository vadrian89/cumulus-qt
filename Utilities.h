#ifndef UTILITIES_H
#define UTILITIES_H

#include <QObject>
#include <QSettings>
#include <QPoint>
#include <QSize>

class Utilities : public QObject{
    Q_OBJECT
public:
    explicit Utilities(QObject *parent = 0);
    Q_INVOKABLE void saveWindowLocation(QPoint position);
    Q_INVOKABLE QPoint loadWindowLocation();
    Q_INVOKABLE void saveWindowSize(QSize size);
    Q_INVOKABLE QSize loadWindowSize();
    Q_INVOKABLE void toggleLauncherCount(QString string);
    Q_INVOKABLE void setLauncherCount(int temperature);
};

#endif // UTILITIES_H
