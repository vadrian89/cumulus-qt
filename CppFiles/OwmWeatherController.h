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
#ifndef OWMWEATHERCONTROLLER_H
#define OWMWEATHERCONTROLLER_H

#include "AbstractWeatherController.h"

class OwmWeatherController : public AbstractWeatherController {
    Q_OBJECT
    void saveWeatherToDb(const QJsonObject &jsonObject);
    void saveForecastToDb(const QJsonObject &jsonObject);
    void readJsonData(QJsonObject jsonObject);
    void getForecast(const QString &code);
public:
    explicit OwmWeatherController(QObject *parent = 0);
    void searchByLocation(QString &location);
    void searchBycode(QString &code);
};

#endif // OWMWEATHERCONTROLLER_H
