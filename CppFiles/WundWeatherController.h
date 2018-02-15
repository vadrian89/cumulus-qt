#ifndef WUNDWEATHERCONTROLLER_H
#define WUNDWEATHERCONTROLLER_H

#include "AbstractWeatherController.h"

class WundWeatherController : public AbstractWeatherController {
    Q_OBJECT
    QDateTime sunrise, sunset;
    void getWeatherFromJson(const QJsonObject &jsonObject);
    void getForecastFromJson(const QJsonObject &jsonObject);
    void getLocationFromJson(const QJsonObject &jsonObject);
    void getForecast(const QString &code);
    void getAfterAstronomy(const QString &code);
    QDate dateFromJson(const QJsonObject &jsonObject);
public:
    explicit WundWeatherController(QObject *parent = nullptr);
    void searchByLocation(QString &location);
    void searchBycode(QString &code);
private slots:
    void getAstronomyFromJson(const QJsonObject &jsonObject);
};

#endif // WUNDWEATHERCONTROLLER_H
