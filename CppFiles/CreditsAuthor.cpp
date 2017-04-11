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
#include "CreditsAuthor.h"

CreditsAuthor::CreditsAuthor(QObject *parent) : QObject(parent){}

void CreditsAuthor::setAuthor(const QString &author) {
    if (m_author != author) {
        m_author = author;
        emit authorChanged();
    }
}

QString CreditsAuthor::author() const {
    return m_author;
}

void CreditsAuthor::setAuthorUrl(const QString &authorUrl) {
    if (m_authorUrl != authorUrl) {
        m_authorUrl = authorUrl;
        emit authorUrlChanged();
    }
}

QString CreditsAuthor::authorUrl() const {
    return m_authorUrl;
}

void CreditsAuthor::setIcon(const QString &icon) {
    if (m_icon != icon) {
        m_icon = icon;
        emit iconChanged();
    }
}

QString CreditsAuthor::icon() const {
    return m_icon;
}

void CreditsAuthor::setLicense(const QString &license) {
    if (m_license!= license) {
        m_license = license;
        emit licenseChanged();
    }
}

QString CreditsAuthor::license() const {
    return m_license;
}
