#  Copyright (C) 2016 Adrian Verban <verbanady@gmail.com>
#  Maintainers: Adrian Verban, Archisman Panigrahi, Daryl Bennett
#  Derived from Typhoon by Archisman Panigrahi which is based on Stormcloud by Jono Cooper <jonocooper.com>
#  Thanks to all the contributors.
#  Using the Ubuntu Condensed font.
#  Thanks to Adam Whitcroft <adamwhitcroft.com> for Climacons!
#  This file is part of Cumulus.
#
#  Cumulus is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Cumulus is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Cumulus.  If not, see <http://www.gnu.org/licenses/>.

TEMPLATE = app

QT += qml quick webengine dbus
CONFIG += c++11

SOURCES += main.cpp \
    Utilities.cpp \
    ThreadWorker.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/lib\'"

HEADERS += \
    Utilities.h \
    ThreadWorker.h

DISTFILES += \
    README.md

