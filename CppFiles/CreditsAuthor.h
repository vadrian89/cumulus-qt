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
#ifndef CREDITSAUTHOR_H
#define CREDITSAUTHOR_H

#include <QObject>

class CreditsAuthor : public QObject {
    Q_OBJECT
    QString m_author, m_authorUrl, m_icon, m_license;
    Q_PROPERTY(QString author MEMBER m_author NOTIFY authorChanged)
    Q_PROPERTY(QString authorUrl MEMBER m_authorUrl NOTIFY authorUrlChanged)
    Q_PROPERTY(QString icon MEMBER m_icon NOTIFY iconChanged)
    Q_PROPERTY(QString license MEMBER m_license NOTIFY licenseChanged)
public:
    explicit CreditsAuthor(QObject *parent = nullptr);
    explicit CreditsAuthor(QObject *parent = nullptr, QString author = "", QString authorUrl = "", QString icon = "", QString license = "");
signals:
    void authorChanged();
    void authorUrlChanged();
    void iconChanged();
    void licenseChanged();
};

#endif // CREDITSAUTHOR_H
