/*
* Copyright (C) 2016 Adrian Verban <verbanady@gmail.com>
* Maintainers: Adrian Verban, Archisman Panigrahi, Daryl Bennett
* Derived from Typhoon by Archisman Panigrahi which is based on Stormcloud by Jono Cooper <jonocooper.com>
* Thanks to all the contributors.
* Using the Ubuntu Condensed font.
* Thanks to Adam Whitcroft <adamwhitcroft.com> for Climacons!
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
