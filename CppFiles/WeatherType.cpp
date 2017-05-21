/*
* Copyright (C) 2016 - 2017 Adrian Verban <verbanady@gmail.com>
* Maintainers: Adrian Verban <verbanady@gmail.com>
* Derived from Typhoon by Archisman Panigrahi which is based on Stormcloud by Jono Cooper <jonocooper.com>
* Thanks to all the contributors.
* Using the Ubuntu Condensed font.
* This file is part of Cumulus.
#
* Cumulus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
#
* Cumulus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
#
* You should have received a copy of the GNU General Public License
* along with Cumulus.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "WeatherType.h"
#include "Forecast.h"
#include "Util.h"

#include <QThread>
#include <QDebug>
WeatherType::WeatherType(QObject *parent) : QObject(parent){
    yahooIcons = new QMap<int, QString>();
    owmIcons = new QMap<int, QString>();
    nightFonts = new QMap<QString, QString>();
    mapYahooIcon();
    mapOwmIcons();
    mapNightIcons();
    setWeatherData();
    m_weatherApi = Util::getWeatherApi();
}

void WeatherType::getWeatherData(){
    qDebug() << "WeatherType::getWeatherData >> Weather data request.";
    QMap<QString, QString> location = searchCriteria();       

    if (location.size() > 0) {
        if (Util::getWeatherApi() == "y") {
            yweather = new YWeatherController();
            connect(yweather, SIGNAL(forecastChanged()), this, SLOT(setWeatherData()));
            connect(yweather, SIGNAL(dataDownloaded()), this, SIGNAL(dataDownloadFinished()));
            if (location.find("code") != location.end())
                yweather->searchBycode(location.find("code").value());
            else
                yweather->searchByLocation(location.find("name").value());
        }
        else {
            owmWeather = new OwmWeatherController();
            connect(owmWeather, SIGNAL(forecastChanged()), this, SLOT(setWeatherData()));
            connect(owmWeather, SIGNAL(dataDownloaded()), this, SIGNAL(dataDownloadFinished()));            
            if (location.find("code") != location.end())
                owmWeather->searchBycode(location.find("code").value());
            else
                owmWeather->searchByLocation(location.find("name").value());
        }
    }
    else {
        emit noLocationSet();
    }
}

void WeatherType::setWeatherData() {
    DbConnection *db = new DbConnection();
    setLocation("Undefined");
    if (db->startCon() == true) {
        int dbLocationId = locationId();
        if (dbLocationId != -1) {
            QString str = "select w_weather_id as weather_id, w_weather_code as weather_code, "
                          "w_description as description, w_temperature as temperature, w_pressure as pressure, "
                          "w_humidity as humidity, w_wind_speed as wind_speed, w_wind_degree as wind_degree, "
                          "w_sunrise as sunrise, w_sunset as sunset, "
                          "(select forec_temp_min from sw_tr_forecast where forec_loc_id = :loc_id order by forec_date limit 1) as temp_min, "
                          "(select forec_temp_max from sw_tr_forecast where forec_loc_id = :loc_id order by forec_date limit 1) as temp_max, "
                          "w_link as link "
                          "from sw_tr_weather where w_loc_id = :loc_id";
            QSqlQuery query;
            query.prepare(str);
            query.bindValue(":loc_id", dbLocationId);
            query.exec();
            if (query.next()) {
                loadData(query);
                emit weatherDataChanged();
            }
            else {
                loadData();
                emit weatherDataChanged();
            }
            if (query.lastError().isValid()) {
                qDebug() << "WeatherType::setWeatherData query error: " << query.lastError().text();
            }
        }
        else {
            loadData();
            emit weatherDataChanged();
        }
    }
    else {
        qDebug() << "WeatherType::setWeatherData database error: " << db->getError();
    }
    db->stopCon();
    db->deleteLater();
}

QList<QObject*> WeatherType::getForecastData() {
    DbConnection *db = new DbConnection();
    QList<QObject*> list;
    Forecast* forecast = NULL;
    if (db->startCon() == true) {
        int dbLocationId = locationId();
        if (dbLocationId != -1) {
            QString str = "select forec_id as forec_id, forec_date as date, forec_weather_code as weather_code, "
                          "forec_weather_description as description, forec_temp_min as temp_min, "
                          "forec_temp_max as temp_max "
                          "from sw_tr_forecast where forec_loc_id = :loc_id limit 1, (select count(*) from sw_tr_forecast where forec_loc_id = :loc_id)";
            QSqlQuery query;
            query.prepare(str);
            query.bindValue(":loc_id", dbLocationId);
            query.exec();
            while (query.next()) {
                forecast = new Forecast();
                forecast->setForecastIndex(query.value("forec_id").toInt());
                forecast->setWeatherCode(findIcon(query.value("weather_code").toInt()));
                forecast->setForecastDesc(query.value("description").toString());
                forecast->setTempLow(query.value("temp_min").toInt());
                forecast->setTempHigh(query.value("temp_max").toInt());
                forecast->setForecastDate(query.value("date").toDate().toString("ddd").toUpper());
                list.append(forecast);
            }
            if (query.lastError().isValid()) {
                qDebug() << "WeatherType::getForecastData query error: " << query.lastError().text();
            }
        }
    }
    else {
        qDebug() << "WeatherType::getForecastData database error: " << db->getError();
    }
    db->stopCon();
    db->deleteLater();
    setForecastList(list);
    return list;
}

int WeatherType::locationId() {
    QString string = "select loc_id as id, loc_name as name from sw_ma_location";
    QSqlQuery query;
    query.exec(string);
    if (query.next()) {
        setLocation(query.value("name").toString());
        return query.value("id").toInt();
    }
    else {
        return -1;
    }
}

void WeatherType::loadData() {
    setWeatherCode(-1);
    setWeatherIcon(findIcon(-1));
    setWeatherDescription("No weather data");
    setTemperature(-1);
    setPressure((float)-1);
    setHumidity((float)-1);
    setWindSpeed(-1);
    setWindDegree(0);
    setPressure(0.0);
    setSunrise("00:00");
    setSunset("00:00");
    setTempMax(-1);
    setTempMin(-1);
    setTempUnit(Util::temperatureUnitSymbol());
    setSpeedUnit(Util::speedUnitSymbol());
    setLocationLink("");
}

void WeatherType::loadData(QSqlQuery &query) {
    setWeatherCode(query.value("weather_id").toInt());
    setWeatherIcon(findIcon(query.value("weather_code").toInt()));
    if (QTime::currentTime() > QTime::fromString(query.value("sunset").toString(), "HH:mm"))
        setWeatherIcon(findNightIcon(findIcon(query.value("weather_code").toInt())));
    else
        setWeatherIcon(findIcon(query.value("weather_code").toInt()));
    setWeatherDescription(query.value("description").toString());
    setTemperature(query.value("temperature").toInt());
    setPressure(query.value("pressure").toDouble());
    setHumidity(query.value("humidity").toFloat());
    setWindSpeed(query.value("wind_speed").toInt());
    setWindDegree(query.value("wind_degree").toInt());
    setSunrise(query.value("sunrise").toString());
    setSunset(query.value("sunset").toString());
    setTempMax(query.value("temp_max").toInt());
    setTempMin(query.value("temp_min").toInt());
    setTempUnit(Util::temperatureUnitSymbol());
    setSpeedUnit(Util::speedUnitSymbol());
    setLocationLink(query.value("link").toString());
}

void WeatherType::mapYahooIcon() {
    yahooIcons->insert(0,"\uf056"); //tornado
    yahooIcons->insert(1,"\uf01d"); //tropical storm
    yahooIcons->insert(2,"\uf073"); // hurricane
    yahooIcons->insert(3,"\uf01e");// severe thunderstorms
    yahooIcons->insert(4,"\uf01e"); // thunderstorms
    yahooIcons->insert(5,"\uf006"); // mixed rain and snow
    yahooIcons->insert(6,"\uf0b2"); // mixed rain and sleet
    yahooIcons->insert(7,"\uf0b2"); // mixed snow and sleet
    yahooIcons->insert(8,"\uf01c"); // freezing drizzle
    yahooIcons->insert(9,"\uf01c"); // drizzle
    yahooIcons->insert(10,"\uf019"); // freezing rain
    yahooIcons->insert(11,"\uf01a"); // showers
    yahooIcons->insert(12,"\uf01a"); // showers
    yahooIcons->insert(13,"\uf064"); // snow flurries
    yahooIcons->insert(14,"\uf064"); // light snow showers
    yahooIcons->insert(15,"\uf064"); // blowing snow
    yahooIcons->insert(16,"\uf064"); // snow
    yahooIcons->insert(17,"\uf015"); // hail
    yahooIcons->insert(18,"\uf0b5"); // sleet
    yahooIcons->insert(19,"\uf063"); // dust
    yahooIcons->insert(20,"\uf014"); // foggy
    yahooIcons->insert(21,"\uf0b6"); // haze
    yahooIcons->insert(22,"\uf062"); // smoky
    yahooIcons->insert(23,"\uf050"); // blustery
    yahooIcons->insert(24,"\uf021"); // windy
    yahooIcons->insert(25,"\uf076"); // cold
    yahooIcons->insert(26,"\uf013"); // cloudy
    yahooIcons->insert(27,"\uf086"); // mostly cloudy (night)
    yahooIcons->insert(28,"\uf002"); // mostly cloudy (day)
    yahooIcons->insert(29,"\uf081"); // partly cloudy (night)
    yahooIcons->insert(30,"\uf002"); // partly cloudy (day)
    yahooIcons->insert(31,"\uf02e"); // clear (night)
    yahooIcons->insert(32,"\uf00d"); // sunny
    yahooIcons->insert(33,"\uf02e"); // fair (night)
    yahooIcons->insert(34,"\uf00d"); // fair (day)
    yahooIcons->insert(35,"\uf017"); // mixed rain and hail
    yahooIcons->insert(36,"\uf072"); // hot
    yahooIcons->insert(37,"\uf01e"); // isolated thunderstorms
    yahooIcons->insert(38,"\uf01e"); // scattered thunderstorms
    yahooIcons->insert(39,"\uf01e"); // scattered thunderstorms
    yahooIcons->insert(40,"\uf01e"); // scattered showers
    yahooIcons->insert(41,"\uf01b"); // heavy snow
    yahooIcons->insert(42,"\uf01b"); // scattered snow showers
    yahooIcons->insert(43,"\uf01b"); // heavy snow
    yahooIcons->insert(44,"\uf002"); // partly cloudy
    yahooIcons->insert(45,"\uf06b"); // thundershowers
    yahooIcons->insert(46,"\uf01b"); // snow showers
    yahooIcons->insert(47,"\uf01e"); // isolated thundershowers
    yahooIcons->insert(-1,"\uf07b");
}

void WeatherType::mapOwmIcons() {
    owmIcons->insert(0,"\uf056"); //tornado
    //Thunderstorms icons
    owmIcons->insert(200, "\uf00e"); //thunderstorm with light rain
    owmIcons->insert(201, "\uf00e"); //thunderstorm with rain
    owmIcons->insert(202, "\uf010"); //thunderstorm with heavy rain
    owmIcons->insert(210, "\uf00e"); //light thunderstorm
    owmIcons->insert(211, "\uf010"); //thunderstorm
    owmIcons->insert(212, "\uf010"); //heavy thunderstorm
    owmIcons->insert(221, "\uf00e"); //ragged thunderstorm
    owmIcons->insert(230, "\uf00e"); //thunderstorm with light drizzle
    owmIcons->insert(231, "\uf00e"); //thunderstorm with drizzle
    owmIcons->insert(232, "\uf00e"); //thunderstorm with heavy drizzle
    //Drizzle icons
    owmIcons->insert(300, "\uf00b"); //light intensity drizzle
    owmIcons->insert(301, "\uf00b"); //drizzle
    owmIcons->insert(302, "\uf00b"); //heavy intensity drizzle
    owmIcons->insert(310, "\uf00b"); //light intensity drizzle rain
    owmIcons->insert(311, "\uf009"); //drizzle rain
    owmIcons->insert(312, "\uf009"); //heavy intensity drizzle rain
    owmIcons->insert(313, "\uf009"); //shower rain and drizzle
    owmIcons->insert(314, "\uf009"); //heavy shower rain and drizzle
    owmIcons->insert(321, "\uf009"); //shower drizzle
    //Rain
    owmIcons->insert(500, "\uf009"); //light rain
    owmIcons->insert(501, "\uf009"); //moderate rain
    owmIcons->insert(502, "\uf009"); //heavy intensity rain
    owmIcons->insert(503, "\uf009"); //very heavy rain
    owmIcons->insert(504, "\uf008"); //extreme rain
    owmIcons->insert(511, "\uf008"); //freezing rain
    owmIcons->insert(520, "\uf008"); //light intensity shower rain
    owmIcons->insert(521, "\uf009"); //shower rain
    owmIcons->insert(522, "\uf009"); //heavy intensity shower rain
    owmIcons->insert(531, "\uf009"); //ragged shower rain
    //Snow
    owmIcons->insert(600, "\uf00a"); //light snow
    owmIcons->insert(601, "\uf00a"); //snow
    owmIcons->insert(602, "\uf00a"); //heavy snow
    owmIcons->insert(611, "\uf0b2"); //sleet
    owmIcons->insert(612, "\uf0b2"); //shower sleet
    owmIcons->insert(615, "\uf0b2"); //light rain and snow
    owmIcons->insert(616, "\uf006"); //rain and snow
    owmIcons->insert(620, "\uf00a"); //light shower snow
    owmIcons->insert(621, "\uf065"); //shower snow
    owmIcons->insert(622, "\uf065"); //heavy shower snow
    //Atmosphere
    owmIcons->insert(701, "\uf003"); //mist
    owmIcons->insert(711, "\uf062"); //smoke
    owmIcons->insert(721, "\uf0b6"); //haze
    owmIcons->insert(731, "\uf063"); //sand, dust whirls
    owmIcons->insert(741, "\uf003"); //fog
    owmIcons->insert(751, "\uf063"); //sand
    owmIcons->insert(761, "\uf063"); //dust
    owmIcons->insert(762, "\uf0c8"); //volcanic ash
    owmIcons->insert(771, "\uf050"); //squalls
    owmIcons->insert(781, "\uf056"); //tornado
    //Clear
    owmIcons->insert(800, "\uf00d"); //clear sky
    //Clouds
    owmIcons->insert(801, "\uf002"); //few clouds
    owmIcons->insert(802, "\uf002"); //scattered clouds
    owmIcons->insert(803, "\uf002"); //broken clouds
    owmIcons->insert(804, "\uf002"); //overcast clouds
    //Extreme
    owmIcons->insert(900, "\uf056"); //tornado
    owmIcons->insert(901, "\uf01d"); //tropical storm
    owmIcons->insert(902, "\uf073"); //hurricane
    owmIcons->insert(903, "\uf076"); //cold
    owmIcons->insert(904, "\uf072"); //hot
    owmIcons->insert(905, "\uf021"); //windy
    owmIcons->insert(906, "\uf004"); //hail
    //Additional
    owmIcons->insert(951, "\uf00d"); //calm
    owmIcons->insert(952, "\uf0c4"); //light breeze
    owmIcons->insert(953, "\uf0c4"); //gentle breeze
    owmIcons->insert(954, "\uf0c4"); //moderate breeze
    owmIcons->insert(955, "\uf0c4"); //fresh breeze
    owmIcons->insert(956, "\uf021"); //strong breeze
    owmIcons->insert(957, "\uf021"); //high wind, near gale
    owmIcons->insert(958, "\uf050"); //gale
    owmIcons->insert(959, "\uf050"); //severe gale
    owmIcons->insert(960, "\uf010"); //storm
    owmIcons->insert(961, "\uf010"); //violent storm
    owmIcons->insert(962, "\uf073"); //hurricane
    owmIcons->insert(-1,"\uf07b");
}

QString WeatherType::findIcon(int weatherCode) {
    if (Util::getWeatherApi() == "y") {
        if (yahooIcons->contains(weatherCode))
            return yahooIcons->find(weatherCode).value();
        else
            return yahooIcons->find(-1).value();
    }
    else {
        if (owmIcons->contains(weatherCode))
            return owmIcons->find(weatherCode).value();
        else
            return owmIcons->find(-1).value();
    }
}

QMap<QString,QString> WeatherType::searchCriteria() {
    DbConnection *db = new DbConnection();
    QMap<QString,QString> map;
    if (db->startCon()) {
        QString string = "select loc_id as id, loc_code as code, loc_name as name from sw_ma_location";
        QSqlQuery query;
        query.exec(string);
        if (query.next()) {
            if (query.value("code").toString().trimmed().size() > 0) {
                map.insert("code", query.value("code").toString());
            }
            else {
                map.insert("name", query.value("name").toString());
            }
        }
    }
    return map;
}

void WeatherType::setWeather(const QString &weather) {
    if (weather != m_weather) {
        m_weather = weather;
        emit weatherChanged();
    }
}
void WeatherType::setWeatherCode(const int &weatherCode) {
    if (weatherCode != m_weatherCode) {
        m_weatherCode = weatherCode;
        emit weatherCodeChanged();
    }
}

void WeatherType::setWeatherIcon(const QString &weatherIcon) {
    if (weatherIcon != m_weatherIcon) {
        m_weatherIcon = weatherIcon;
        emit weatherIconChanged();
    }
}

QString WeatherType::weatherIcon() const {
    return m_weatherIcon;
}

void WeatherType::setWeatherDescription(const QString &weatherDescription) {
    if (weatherDescription != m_weatherDescription) {
        m_weatherDescription = weatherDescription;
        emit weatherDescriptionChanged();
    }
}

QString WeatherType::weatherDescription() const {
    return m_weatherDescription;
}

void WeatherType::setTemperature(const int &temp) {
    if (temp != m_temperature) {
        m_temperature = temp;
        emit temperatureChanged();
    }
}

int WeatherType::temperature() const {
    return m_temperature;
}

void WeatherType::setPressure(const double &pressure) {
    if (m_pressure != pressure) {
        m_pressure = QString::number(pressure, 'd', 2).toDouble();
        emit pressureChanged();
    }
}

double WeatherType::pressure() const {
    return m_pressure;
}

void WeatherType::setHumidity(const float &humidity) {
    if (humidity != m_humidity) {
        m_humidity = humidity;
        emit humidityChanged();
    }
}

float WeatherType::humidity() const {
    return m_humidity;
}

void WeatherType::setWindSpeed(const int &windSpeed) {
    if (windSpeed!= m_windSpeed) {
        m_windSpeed = windSpeed;
        emit windSpeedChanged();
    }
}

int WeatherType::windSpeed() const {
    return m_windSpeed;
}

void WeatherType::setWindDegree(const int &windDegree) {
    if (windDegree != m_windDegree) {
        m_windDegree = windDegree;
        emit windDegreeChanged();
    }
}

int WeatherType::windDegree() const {
    return m_windDegree;
}

void WeatherType::setLocation(const QString &location) {
    if (location != m_location) {
        m_location = location;
        emit locationChanged();
    }
}

void WeatherType::setLocationLink(const QString &locationLink) {
    if (locationLink != m_locationLink) {
        m_locationLink = locationLink;
        emit locationLinkChanged();
    }
}

QString WeatherType::weather() const {
    return m_weather;
}
int WeatherType::weatherCode() const {
    return m_weatherCode;
}

QString WeatherType::location() const {
    return m_location;
}
QString WeatherType::locationLink() const {
    return m_locationLink;
}

void WeatherType::setSunrise(const QString &sunrise) {
    if (m_sunrise != sunrise) {
        m_sunrise = sunrise;
        emit sunriseChanged();
    }
}

QString WeatherType::sunrise() const {
    return m_sunrise;
}

void WeatherType::setSunset(const QString &sunset) {
    if (m_sunset != sunset) {
        m_sunset = sunset;
        emit sunsetChanged();
    }
}

QString WeatherType::sunset() const {
    return m_sunset;
}

void WeatherType::setTempMax(const int &tempMax) {
    if (m_tempMax != tempMax) {
        m_tempMax = tempMax;
        emit tempMaxChanged();
    }
}

int WeatherType::tempMax() const {
    return m_tempMax;
}

void WeatherType::setTempMin(const int &tempMin) {
    if (m_tempMin != tempMin) {
        m_tempMin = tempMin;
        emit tempMinChanged();
    }
}

int WeatherType::tempMin() const {
    return m_tempMin;
}

void WeatherType::setLoadFinished(const bool &loadFinished) {
    if (m_loadFinished != loadFinished) {
        m_loadFinished = loadFinished;
        if (m_loadFinished == true) {
            if(Util::getWeatherApi() == "y")
                yweather->saveDataToDb();
            else
                owmWeather->saveDataToDb();
        }
        emit loadFinishedChanged();
    }
}

bool WeatherType::loadFinished() const {
    return m_loadFinished;
}

QString WeatherType::tempUnit() const {
    return m_tempUnit;
}

void WeatherType::setTempUnit(const QString &tempUnit) {
    if (tempUnit != m_tempUnit) {
        m_tempUnit = tempUnit;
        emit tempUnitChanged();
    }
}

void WeatherType::changeTempUnit(const QString &unit) {
    if (m_tempUnit != unit) {
        QString oldUnit = Util::getTemperatureUnit().toLower();
        Util::setTemperatureUnit(unit);
        setTemperature(Util::calculateTemperature(m_temperature, oldUnit));
        setTempMax(Util::calculateTemperature(m_tempMax, oldUnit));
        setTempMin(Util::calculateTemperature(m_tempMin, oldUnit));
        updateForecastTemp(oldUnit);
        setTempUnit(Util::temperatureUnitSymbol());
    }
}

void WeatherType::updateForecastTemp(const QString &oldUnit) {
    for (int i = 0; i < m_forecastList.size(); i++) {
        Forecast *f = (Forecast*)m_forecastList.at(i);
        f->setTempHigh(Util::calculateTemperature(f->tempHigh(), oldUnit));
        f->setTempLow(Util::calculateTemperature(f->tempLow(), oldUnit));
    }
    emit forecastListChanged();
}

QList<QObject*> WeatherType::forecastList() const {
    return m_forecastList;
}

void WeatherType::setForecastList(const QList<QObject*> &list) {
    if (m_forecastList != list) {
        m_forecastList = list;
        emit forecastListChanged();
    }
}

void WeatherType::changeSpeedUnit(const QString &unit) {
    if (m_speedUnit != unit) {
        QString oldUnit = Util::getWindSpeedUnit().toLower();
        Util::setWindSpeedUnit(unit);
        setWindSpeed(Util::calculateWindSpeed(m_windSpeed, oldUnit));
        setSpeedUnit(Util::speedUnitSymbol());
    }
}

QString WeatherType::speedUnit() const {
    return m_speedUnit;
}

void WeatherType::setSpeedUnit(const QString &unit) {
    if (m_speedUnit != unit) {
        m_speedUnit = unit;
        emit speedUnitChanged();
    }
}

QString WeatherType::findNightIcon(const QString fontCode) const{
    if (nightFonts->contains(fontCode))
        return nightFonts->find(fontCode).value();
    else
        return fontCode;
}

void WeatherType::mapNightIcons() {
    nightFonts->insert("\uf00d", "\uf02e");
    nightFonts->insert("\uf002", "\uf086");
    nightFonts->insert("\uf000", "\uf022");
    nightFonts->insert("\uf003", "\uf04a");
    nightFonts->insert("\uf004", "\uf024");
    nightFonts->insert("\uf0b6", "\uf0b6");
    nightFonts->insert("\uf005", "\uf033");
    nightFonts->insert("\uf008", "\uf036");
    nightFonts->insert("\uf006", "\uf026");
    nightFonts->insert("\uf007", "\uf027");
    nightFonts->insert("\uf009", "\uf029");
    nightFonts->insert("\uf0b2", "\uf0b4");
    nightFonts->insert("\uf068", "\uf06a");
    nightFonts->insert("\uf00a", "\uf02a");
    nightFonts->insert("\uf06b", "\uf06d");
    nightFonts->insert("\uf065", "\uf067");
    nightFonts->insert("\uf00b", "\uf02b");
    nightFonts->insert("\uf00e", "\uf02c");
    nightFonts->insert("\uf00c", "\uf083");
    nightFonts->insert("\uf010", "\uf03b");
    nightFonts->insert("\uf085", "\uf021");
    nightFonts->insert("\uf06e", "\uf070");
    nightFonts->insert("\uf072", "\uf077");
    nightFonts->insert("\uf07d", "\uf080");
    nightFonts->insert("\uf0c4", "\uf021");
}

bool WeatherType::clearLocationCode() {
    QString string = "update sw_ma_location set loc_code = '' where loc_id = :loc_id";
    DbConnection *db = new DbConnection();
    if (db->startCon()) {
        QSqlQuery query;
        query.prepare(string);
        query.bindValue(":loc_id", 1);
        if (query.exec()) {
            return true;
        }
        else {
            qDebug() << "WeatherType::clearLocationCode() query error: " << query.lastError().text();
            return false;
        }
    }
    else {
        qDebug() << "WeatherType::clearLocationCode() db error: " << db->getError();
        return false;
    }    
}

QString WeatherType::weatherApi() const {
    return m_weatherApi;
}

#include <QDebug>
void WeatherType::setWeatherApi(const QString &weatherApi) {
    if (m_weatherApi != weatherApi && clearLocationCode()) {
        m_weatherApi = weatherApi;
        Util::setWeatherApi(weatherApi);
        emit weatherApiChanged();
    }
}
