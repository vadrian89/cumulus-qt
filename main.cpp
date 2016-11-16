#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine>
#include <QSettings>
#include <QQmlContext>

#include "Utilities.h"
int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationName("Visoft");
    QCoreApplication::setApplicationName("Cumulus");
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QtWebEngine::initialize();

    Utilities utilities;
    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("util", &utilities);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
