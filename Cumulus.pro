QT += qml quick sql widgets

CONFIG += c++11

SOURCES += main.cpp \
    CppFiles/Util.cpp \
    CppFiles/DbConnection.cpp \
    CppFiles/DataController.cpp \
    CppFiles/YWeatherController.cpp \
    CppFiles/Forecast.cpp \
    CppFiles/SettingsController.cpp \
    CppFiles/SearchLocation.cpp \
    CppFiles/OwmWeatherController.cpp \
    CppFiles/ThreadWorker.cpp \
    CppFiles/WeatherType.cpp \
    CppFiles/AbstractWeatherController.cpp \
    CppFiles/TrayController.cpp \
    CppFiles/CreditsAuthor.cpp \
    CppFiles/CustomImageProvider.cpp \
    CppFiles/FontImageProvider.cpp \
    CppFiles/WundWeatherController.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    CppFiles/Util.h \
    CppFiles/DbConnection.h \
    CppFiles/DataController.h \
    CppFiles/YWeatherController.h \
    CppFiles/Forecast.h \
    CppFiles/SettingsController.h \
    CppFiles/SearchLocation.h \
    CppFiles/OwmWeatherController.h \
    CppFiles/ThreadWorker.h \
    CppFiles/WeatherType.h \
    CppFiles/AbstractWeatherController.h \
    CppFiles/TrayController.h \
    CppFiles/CreditsAuthor.h \
    CppFiles/CustomImageProvider.h \
    CppFiles/FontImageProvider.h \
    CppFiles/WundWeatherController.h

QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/lib\'"

DISTFILES += \
    README.md \
    .travis.yml \
    _config.yml \
    docs/_config.yml \
    docs/assets/images/weather.gif \
    docs/assets/images/location.png \
    docs/assets/images/settings.png \
    docs/assets/images/weather.png \
    docs/assets/images/weather_green.png \
    docs/assets/images/weather_purp.png \
    docs/assets/images/weather_red.png \
    docs/index.md \
    docs/multiple_inst.md

MOC_DIR = .moc
OBJECTS_DIR = .obj
