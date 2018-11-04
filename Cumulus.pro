QT += qml quick sql widgets quickwidgets concurrent positioning

CONFIG += c++11

SOURCES += \
    CppFiles/AbstractWeatherController.cpp \
    CppFiles/CreditsAuthor.cpp \
    CppFiles/CustomImageProvider.cpp \
    CppFiles/DatabaseHelper.cpp \
    CppFiles/DataController.cpp \
    CppFiles/FontImageProvider.cpp \
    CppFiles/Forecast.cpp \
    CppFiles/Location.cpp \
    CppFiles/LocationController.cpp \
    CppFiles/MainWindow.cpp \
    CppFiles/OwmWeatherController.cpp \
    CppFiles/SettingsController.cpp \
    CppFiles/ThreadWorker.cpp \
    CppFiles/TrayController.cpp \
    CppFiles/Util.cpp \
    CppFiles/WeatherType.cpp \
    CppFiles/WundWeatherController.cpp \
    CppFiles/YWeatherController.cpp \
    main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    CppFiles/AbstractWeatherController.h \
    CppFiles/CreditsAuthor.h \
    CppFiles/CustomImageProvider.h \
    CppFiles/DatabaseHelper.h \
    CppFiles/DataController.h \
    CppFiles/FontImageProvider.h \
    CppFiles/Forecast.h \
    CppFiles/Location.h \
    CppFiles/LocationController.h \
    CppFiles/MainWindow.h \
    CppFiles/OwmWeatherController.h \
    CppFiles/SettingsController.h \
    CppFiles/ThreadWorker.h \
    CppFiles/TrayController.h \
    CppFiles/Util.h \
    CppFiles/WeatherType.h \
    CppFiles/WundWeatherController.h \
    CppFiles/YWeatherController.h

#QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/lib\'"

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
    assets/icons/hicolor/128x128/api.png \
    assets/icons/hicolor/128x128/credits.png \
    assets/icons/hicolor/128x128/cumulus.png \
    assets/icons/hicolor/128x128/map.png \
    assets/icons/hicolor/128x128/palette.png \
    assets/icons/hicolor/128x128/speed_icon.png \
    assets/icons/hicolor/256x256/api.png \
    assets/icons/hicolor/256x256/credits.png \
    assets/icons/hicolor/256x256/cumulus.png \
    assets/icons/hicolor/256x256/map.png \
    assets/icons/hicolor/256x256/palette.png \
    assets/icons/hicolor/256x256/speed_icon.png \
    assets/icons/hicolor/256x256/temperature_icon.png \
    assets/icons/hicolor/512x512/api.png \
    assets/icons/hicolor/512x512/credits.png \
    assets/icons/hicolor/512x512/cumulus.png \
    assets/icons/hicolor/512x512/map.png \
    assets/icons/hicolor/512x512/palette.png \
    assets/icons/hicolor/512x512/speed_icon.png \
    assets/icons/hicolor/512x512/temperature_icon.png \
    assets/icons/hicolor/64x64/api.png \
    assets/icons/hicolor/64x64/credits.png \
    assets/icons/hicolor/64x64/cumulus.png \
    assets/icons/hicolor/64x64/map.png \
    assets/icons/hicolor/64x64/palette.png \
    assets/icons/hicolor/64x64/speed_icon.png \
    assets/icons/hicolor/64x64/temperature_icon.png \
    assets/icons/wund_logo.png \
    assets/icons/wund_logo_light.png \
    assets/icons/yw_logo.png \
    docs/index.md \
    docs/multiple_inst.md \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/build.gradle \
    android/gradlew.bat \
    assets/icons/hicolor/128x128/temperature_icon.png \
    assets/icons/hicolor/scalable/cumulus.svg \
    assets/weather_codes.json \
    assets/screens/location-search.png \
    assets/screens/locations-screen.png \
    assets/screens/main-screen-forecast.png \
    assets/screens/toggle-screens.png \
    assets/screens/location-search.png \
    assets/screens/locations-screen.png \
    assets/screens/main-screen-forecast.png \
    assets/screens/toggle-screens.png \
    android/assets/screens/location-search.png \
    android/assets/screens/locations-screen.png \
    android/assets/screens/main-screen-forecast.png \
    android/assets/screens/toggle-screens.png \
    translations/tempestas_ro.qm \
    translations/tempestas_ro.po

MOC_DIR = .moc
OBJECTS_DIR = .obj

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

TRANSLATIONS = translations/tempestas_ro.po
