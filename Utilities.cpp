#include "Utilities.h"
#include <QtDBus>

Utilities::Utilities(QObject *parent) : QObject(parent){}

void Utilities::saveWindowLocation(QPoint position) {
    QSettings settings;
    settings.beginGroup("window-settings");
    settings.setValue("pos", position);
    settings.endGroup();
}

QPoint Utilities::loadWindowLocation() {
    QSettings settings;
    settings.beginGroup("window-settings");
    QPoint pos = settings.value("pos", QPoint(0,0)).toPoint();
    settings.endGroup();
    return pos;
}

void Utilities::saveWindowSize(QSize size) {
    QSettings settings;
    settings.beginGroup("window-settings");
    settings.setValue("size", size);
    settings.endGroup();
}

QSize Utilities::loadWindowSize() {
    QSettings settings;
    settings.beginGroup("window-settings");
    QSize size = settings.value("size", QSize(300,500)).toSize();
    settings.endGroup();
    return size;
}

void Utilities::toggleLauncherCount(QString string) {
    QDBusMessage signal = QDBusMessage::createSignal("/", "com.canonical.Unity.LauncherEntry", "Update");
    signal << "application://cumulus.desktop";
    QVariantMap property;
    if (string.toLower() == "enable_launcher") {
        property.insert("count-visible", true);
    }
    else {
        property.insert("count-visible", false);
    }
    signal << property;
    QDBusConnection::sessionBus().send(signal);
}

void Utilities::setLauncherCount(int temperature) {
    QDBusMessage signal = QDBusMessage::createSignal("/", "com.canonical.Unity.LauncherEntry", "Update");
    signal << "application://cumulus.desktop";
    QVariantMap property;
    property.insert("count", qint64(temperature));
    signal << property;
    QDBusConnection::sessionBus().send(signal);
}
