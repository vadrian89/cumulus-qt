#ifndef WUNDWEATHERCONTROLLER_H
#define WUNDWEATHERCONTROLLER_H

#include "AbstractWeatherController.h"

class WundWeatherController : public AbstractWeatherController {
    Q_OBJECT    
    void getWeatherFromJson(const QJsonObject &jsonObject);
    void getForecastFromJson(const QJsonObject &jsonObject);
    void getLocationFromJson(const QJsonObject &jsonObject);
    void getForecast(const QString &code);
    void getAstronomy(const QString &code);
    QDate dateFromJson(const QJsonObject &jsonObject);
public:
    explicit WundWeatherController(QObject *parent = nullptr);    
    void searchBycode(QString &code);
    void searchByGps(const double &lat, const double &lon);
private slots:
    void searchByLocation(QString &location);
    void getAstronomyFromJson(const QJsonObject &jsonObject);
};

#endif // WUNDWEATHERCONTROLLER_H
