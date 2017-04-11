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
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
    Q_PROPERTY(QString authorUrl READ authorUrl WRITE setAuthorUrl NOTIFY authorUrlChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString license READ license WRITE setLicense NOTIFY licenseChanged)
public:
    explicit CreditsAuthor(QObject *parent = 0);
    void setAuthor(const QString &author);
    QString author() const;
    void setAuthorUrl(const QString &authorUrl);
    QString authorUrl() const;
    void setIcon(const QString &icon);
    QString icon() const;
    void setLicense(const QString &license);
    QString license() const;
signals:
    void authorChanged();
    void authorUrlChanged();
    void iconChanged();
    void licenseChanged();
};

#endif // CREDITSAUTHOR_H
