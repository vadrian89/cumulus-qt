#ifndef WUNDWEATHERCONTROLLER_H
#define WUNDWEATHERCONTROLLER_H

#include "AbstractWeatherController.h"
#include <QMap>

class WundWeatherController : public AbstractWeatherController {
    Q_OBJECT
    QMap<QString, int> *wundIconsCodes;
    QDateTime sunrise, sunset;
    void saveWeatherToDb(const QJsonObject &jsonObject);
    void saveForecastToDb(const QJsonObject &jsonObject);
    void readJsonData(QJsonObject jsonObject);
    void getForecast(const QString &code);
    void getAstronomy(const QString &code);    
    void mapWundIconsCodes();    
    QDate dateFromJson(const QJsonObject &jsonObject);
public:
    explicit WundWeatherController(QObject *parent = nullptr);
    void searchByLocation(QString &location);
    void searchBycode(QString &code);
};

#endif // WUNDWEATHERCONTROLLER_H
