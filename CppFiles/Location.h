/*
* Copyright (C) 2018 Adrian Verban <verbanady@gmail.com>
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
