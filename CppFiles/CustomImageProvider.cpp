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
#include "Util.h"

CustomImageProvider::CustomImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

QImage CustomImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    Q_UNUSED(size)
    Q_UNUSED(requestedSize)
    if (id == "owm-logo") {
        return getImageFromText("Provided by:\nOpenWeatherMap", Util::textColor(), 268, 58);
    }
    else {
        return getImageFromText("", Util::textColor(), 268, 58);
    }
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
