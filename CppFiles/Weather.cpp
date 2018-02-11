#include "Weather.h"
#include "Util.h"

Weather::Weather(QObject *parent) : QObject(parent) {}

void Weather::setWeatherCode(const int &weatherCode) {
    if (weatherCode != m_weatherCode) {
        m_weatherCode = weatherCode;
        emit weatherCodeChanged();
    }
}

int Weather::weatherCode() const {
    return m_weatherCode;
}

void Weather::setWeatherIcon(const QString &weatherIcon) {
    if (weatherIcon != m_weatherIcon) {
        m_weatherIcon = weatherIcon;
        emit weatherIconChanged();
    }
}

QString Weather::weatherIcon() const {
    return m_weatherIcon;
}

void Weather::setWeatherDescription(const QString &weatherDescription) {
    if (weatherDescription != m_weatherDescription) {
        m_weatherDescription = Util::firstLetterUp(weatherDescription);
        emit weatherDescriptionChanged();
    }
}

QString Weather::weatherDescription() const {
    return m_weatherDescription;
}

void Weather::setTemperature(const int &temp) {
    if (temp != m_temperature) {
        m_temperature = temp;
        emit temperatureChanged();
    }
}

int Weather::temperature() const {
    return m_temperature;
}

void Weather::setPressure(const double &pressure) {
    if (m_pressure != pressure) {
        m_pressure = QString::number(pressure, 'd', 2).toDouble();
        emit pressureChanged();
    }
}

double Weather::pressure() const {
    return m_pressure;
}

void Weather::setHumidity(const float &humidity) {
    if (humidity != m_humidity) {
        m_humidity = humidity;
        emit humidityChanged();
    }
}

float Weather::humidity() const {
    return m_humidity;
}

void Weather::setWindSpeed(const int &windSpeed) {
    if (windSpeed!= m_windSpeed) {
        m_windSpeed = windSpeed;
        emit windSpeedChanged();
    }
}

int Weather::windSpeed() const {
    return m_windSpeed;
}

void Weather::setWindDegree(const int &windDegree) {
    if (windDegree != m_windDegree) {
        m_windDegree = windDegree;
        emit windDegreeChanged();
    }
}

int Weather::windDegree() const {
    return m_windDegree;
}

void Weather::setLocation(const QString &location) {
    if (location != m_location) {
        m_location = location;
        emit locationChanged();
    }
}

QString Weather::location() const {
    return m_location;
}

void Weather::setLocationLink(const QString &locationLink) {
    if (locationLink != m_locationLink) {
        m_locationLink = locationLink;
        emit locationLinkChanged();
    }
}

QString Weather::locationLink() const {
    return m_locationLink;
}

void Weather::setSunrise(const QString &sunrise) {
    if (m_sunrise != sunrise) {
        m_sunrise = sunrise;
        emit sunriseChanged();
    }
}

QString Weather::sunrise() const {
    return m_sunrise;
}

void Weather::setSunset(const QString &sunset) {
    if (m_sunset != sunset) {
        m_sunset = sunset;
        emit sunsetChanged();
    }
}

QString Weather::sunset() const {
    return m_sunset;
}

void Weather::setTempMax(const int &tempMax) {
    if (m_tempMax != tempMax) {
        m_tempMax = tempMax;
        emit tempMaxChanged();
    }
}

int Weather::tempMax() const {
    return m_tempMax;
}

void Weather::setTempMin(const int &tempMin) {
    if (m_tempMin != tempMin) {
        m_tempMin = tempMin;
        emit tempMinChanged();
    }
}

int Weather::tempMin() const {
    return m_tempMin;
}

void Weather::setTempUnit(const QString &tempUnit) {
    if (tempUnit != m_tempUnit) {
        m_tempUnit = tempUnit;
        emit tempUnitChanged();
    }
}

QString Weather::tempUnit() const {
    return m_tempUnit;
}

QList<QObject*> Weather::forecastList() const {
    return m_forecastList;
}

void Weather::setForecastList(const QList<QObject*> &list) {
    if (m_forecastList != list) {
        m_forecastList = list;
        emit forecastListChanged();
    }
}

QString Weather::speedUnit() const {
    return m_speedUnit;
}

void Weather::setSpeedUnit(const QString &unit) {
    if (m_speedUnit != unit) {
        m_speedUnit = unit;
        emit speedUnitChanged();
    }
}

QString Weather::weatherApi() const {
    return m_weatherApi;
}

void Weather::setWeatherApi(const QString &weatherApi) {
    if (m_weatherApi != weatherApi) {
        m_weatherApi = weatherApi;
        emit weatherApiChanged();
    }
}

int Weather::locationId() const {
    return m_locationId;
}

void Weather::setLocationId(const int &locationId) {
    if (m_locationId != locationId) {
        m_locationId = locationId;
        emit locationIdChanged();
    }
}
