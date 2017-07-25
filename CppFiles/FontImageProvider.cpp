#include "FontImageProvider.h"
#include <QFont>
#include <QFontDatabase>
#include <QPainter>
#include <QDebug>

FontImageProvider::FontImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

QImage FontImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    int width = 22;
    int height = 22;
    QString text = "no text";
    QString color = "#ffffff";
    QStringList stringList = id.split(QRegExp("[#]"));
    if (stringList.size() < 3) {
        text = stringList.at(0);
        color = "#" + stringList.at(1);
    }
    if (size)
        *size = QSize(width, height);
    return getImageFromText(text, color, requestedSize.width() > 0 ? requestedSize.width() : width,
                            requestedSize.height() > 0 ? requestedSize.height() : height);
}

QImage FontImageProvider::getImageFromText(const QString &text, const QString &color, const int &width, const int &height) {
    int id = QFontDatabase::addApplicationFont(":/fonts/fontawesome.ttf");
    if (id < 0)
        qDebug() << "Font not loaded";
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont fontAwesome(family);
    fontAwesome.setPixelSize(height * 60 / 100);
    QImage image(width, height, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    painter.setFont(fontAwesome);
    painter.setPen(QColor(color));
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.drawText(image.rect(), Qt::AlignCenter, text);
    return image;
}
