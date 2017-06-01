/*
* Copyright (C) 2017 Adrian Verban <verbanady@gmail.com>
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
#include "CustomImageProvider.h"
#include <QFont>
#include <QPainter>

CustomImageProvider::CustomImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

QImage CustomImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    int width = 268;
    int height = 58;
    QStringList stringList = id.split(QRegExp("[#]"));
    QString text = stringList.at(0) + "\n" + stringList.at(1);
    QString color = "#" + stringList.at(2);
    if (size)
        *size = QSize(width, height);
    return getImageFromText(text, color, requestedSize.width() > 0 ? requestedSize.width() : width,
                            requestedSize.height() > 0 ? requestedSize.height() : height);
}

QImage CustomImageProvider::getImageFromText(const QString &text, const QString &color, const int &width, const int &height) {
    QImage image(width, height, QImage::Format_ARGB32_Premultiplied);
    QFont font;
    font.setPixelSize(24);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    painter.setFont(font);
    painter.setPen(QColor(color));
    painter.drawText(image.rect(), Qt::AlignCenter, text);
    return image;
}
