QT += qml quick sql widgets quickwidgets concurrent

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
    CppFiles/WundWeatherController.cpp \
    CppFiles/MainWindow.cpp

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
    CppFiles/WundWeatherController.h \
    CppFiles/MainWindow.h

QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/lib\'"

DISTFILES += \
    README.md \
    .travis.yml \
    _config.yml \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    docs/_config.yml \
    android/res/values/libs.xml \
    docs/assets/images/weather.gif \
    android/assets/api.png \
    android/assets/credits.png \
    android/assets/cumulus.png \
    android/assets/map.png \
    android/assets/palette.png \
    android/assets/speed_icon.png \
    android/assets/temperature_icon.png \
    android/assets/wund_logo.png \
    android/assets/wund_logo_light.png \
    android/assets/yw_logo.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/api.png \
    android/res/drawable-ldpi/credits.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-ldpi/map.png \
    android/res/drawable-ldpi/palette.png \
    android/res/drawable-ldpi/speed_icon.png \
    android/res/drawable-ldpi/temperature_icon.png \
    android/res/drawable-mdpi/api.png \
    android/res/drawable-mdpi/credits.png \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-mdpi/map.png \
    android/res/drawable-mdpi/palette.png \
    android/res/drawable-mdpi/speed_icon.png \
    android/res/drawable-mdpi/temperature_icon.png \
    docs/assets/images/location.png \
    docs/assets/images/settings.png \
    docs/assets/images/weather.png \
    docs/assets/images/weather_green.png \
    docs/assets/images/weather_purp.png \
    docs/assets/images/weather_red.png \
    icons/hicolor/128x128/api.png \
    icons/hicolor/128x128/credits.png \
    icons/hicolor/128x128/cumulus.png \
    icons/hicolor/128x128/map.png \
    icons/hicolor/128x128/palette.png \
    icons/hicolor/128x128/speed_icon.png \
    icons/hicolor/256x256/api.png \
    icons/hicolor/256x256/credits.png \
    icons/hicolor/256x256/cumulus.png \
    icons/hicolor/256x256/map.png \
    icons/hicolor/256x256/palette.png \
    icons/hicolor/256x256/speed_icon.png \
    icons/hicolor/256x256/temperature_icon.png \
    icons/hicolor/512x512/api.png \
    icons/hicolor/512x512/credits.png \
    icons/hicolor/512x512/cumulus.png \
    icons/hicolor/512x512/map.png \
    icons/hicolor/512x512/palette.png \
    icons/hicolor/512x512/speed_icon.png \
    icons/hicolor/512x512/temperature_icon.png \
    icons/hicolor/64x64/api.png \
    icons/hicolor/64x64/credits.png \
    icons/hicolor/64x64/cumulus.png \
    icons/hicolor/64x64/map.png \
    icons/hicolor/64x64/palette.png \
    icons/hicolor/64x64/speed_icon.png \
    icons/hicolor/64x64/temperature_icon.png \
    icons/wund_logo.png \
    icons/wund_logo_light.png \
    icons/yw_logo.png \
    docs/index.md \
    docs/multiple_inst.md \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/build.gradle \
    android/gradlew.bat \
    android/gradle.properties \
    icons/hicolor/128x128/temperature_icon.png \
    icons/hicolor/scalable/cumulus.svg

MOC_DIR = .moc
OBJECTS_DIR = .obj

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
