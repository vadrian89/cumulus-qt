#ifndef WUNDWEATHERCONTROLLER_H
#define WUNDWEATHERCONTROLLER_H

#include "AbstractWeatherController.h"

class WundWeatherController : public AbstractWeatherController {
    Q_OBJECT
    void saveWeatherToDb(const QJsonObject jsonObject);
    void saveForecastToDb(const QJsonObject jsonObject);
    void readJsonData(QJsonObject jsonObject);
    void getForecast(const QString &code);
public:
    explicit WundWeatherController(QObject *parent = nullptr);
    void searchByLocation(QString &location);
    void searchBycode(QString &code);
};

#endif // WUNDWEATHERCONTROLLER_H
