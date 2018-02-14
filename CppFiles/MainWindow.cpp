#include "MainWindow.h"
#include "WeatherType.h"
#include "Forecast.h"
#include "Util.h"
#include "SettingsController.h"
#include "SearchLocation.h"
#include "TrayController.h"
#include "CustomImageProvider.h"
#include "FontImageProvider.h"

#include <QSettings>
#include <QQmlEngine>
#include <QQmlContext>
#include <QApplication>
#include <QQuickWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_closeApp = false;
    QQuickWidget *view = new QQuickWidget();
    QQmlEngine *engine = view->engine();
    connect(engine, SIGNAL(quit()), this, SLOT(closeApp()));
    QQmlContext *context = engine->rootContext();
    Util *util = new Util(view);
    SettingsController *settingsController = new SettingsController(view);
    engine->addImageProvider(QLatin1String("customimage"), new CustomImageProvider());
    engine->addImageProvider(QLatin1String("fontimage"), new FontImageProvider());
    context->setContextProperty("util", util);
    context->setContextProperty("settingsController", settingsController);
    context->setContextProperty("applicationPath", "file://" + qApp->applicationDirPath() + "/");
    context->setContextProperty("applicationWindow", this);
    registerQmlType();
    view->setSource(QUrl(QLatin1String("qrc:/main-desktop.qml")));
    view->show();
    view->setResizeMode(QQuickWidget::SizeRootObjectToView);
    view->setClearColor(Qt::transparent);
    setAttribute(Qt::WA_TranslucentBackground);
    loadSettings();
    setCentralWidget(view);
    setMinimumHeight(150);
    setMinimumWidth(140);
    setWindowIcon(QIcon(Util::iconPathPrefix() + "cumulus.png"));
}

MainWindow::~MainWindow() {}

void MainWindow::loadSettings(){
    QSettings settings;
    settings.beginGroup("app-settings");
    if (!restoreGeometry(settings.value("geo").toByteArray()) && Util::osType() != "android")
        resize(300, 500);
    restoreState(settings.value("state").toByteArray());
    settings.endGroup();
}

void MainWindow::saveSettings(){
    QSettings settings;
    settings.beginGroup("app-settings");
    settings.setValue("geo",saveGeometry());
    settings.setValue("state",saveState());
    settings.endGroup();
}

void MainWindow::registerQmlType() {
    qmlRegisterType<WeatherType>("ownTypes.weather", 1, 9, "Weather");
    qmlRegisterType<SettingsController>("ownTypes.settingsController", 1, 0, "SettingsController");
    qmlRegisterType<SearchLocation>("ownTypes.searchLocation", 0, 4, "LocationSearchController");
    qmlRegisterType<TrayController>("ownTypes.TrayController", 0, 3, "TrayController");
}

void MainWindow::closeEvent(QCloseEvent *event) {
    saveSettings();
    if (!m_closeApp && Util::trayVisibility()) {
        this->hide();
        event->ignore();
    }
    else {
        QMainWindow::closeEvent(event);
    }
}

void MainWindow::launchApp() {
    if (!Util::trayVisibility())
        this->show();
}

void MainWindow::closeApp() {
    m_closeApp = true;
    this->close();
}
