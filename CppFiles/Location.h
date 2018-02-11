#ifndef LOCATION_H
#define LOCATION_H

#include <QObject>

class Location : public QObject {
    Q_OBJECT    
public:
    int m_locationId;
    QString m_locationCode, m_locationName;

    explicit Location(QObject *parent = nullptr);
    explicit Location(QObject *parent = nullptr, int locationId = 1, QString locationCode = "", QString locationName = "");
    Q_PROPERTY(int locationId MEMBER m_locationId)
    Q_PROPERTY(QString locationCode MEMBER m_locationCode)
    Q_PROPERTY(QString locationName MEMBER m_locationName)
};

#endif // LOCATION_H
