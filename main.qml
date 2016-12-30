/*
* Copyright (C) 2016 Adrian Verban <verbanady@gmail.com>
* Maintainers: Adrian Verban, Archisman Panigrahi, Daryl Bennett
* Derived from Typhoon by Archisman Panigrahi which is based on Stormcloud by Jono Cooper <jonocooper.com>
* Thanks to all the contributors.
* Using the Ubuntu Condensed font.
* Thanks to Adam Whitcroft <adamwhitcroft.com> for Climacons!
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
import QtQuick 2.0
import QtQuick.Window 2.2
import QtWebEngine 1.1
import QtQml 2.2

Window {
    id: window
    property point windowLocation: util.loadWindowLocation()
    property size windowSize: util.loadWindowSize()
    flags: Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint
    minimumHeight: windowSize.height
    minimumWidth: windowSize.width
    maximumHeight: windowSize.height
    maximumWidth: windowSize.width
    x: windowLocation.x
    y: windowLocation.y
    visible: false

    WebEngineView {
        property string systemLanguage: Qt.locale().name.substring(0,2)
        anchors.fill: parent
        url: "data/app.html?lang=" + systemLanguage

        Component.onCompleted: {
            window.visible = true
        }

        onJavaScriptConsoleMessage: {
            console.log("Cumulus javascript log: " + message)
            if(message.substring(0,3) == "url") {
                Qt.openUrlExternally(message.substring(3,message.length))
            }
        }
        onTitleChanged: {
            console.log("Cumulus title changed: " + title)
            if (title == "disabledrag") {
                appMouseArea.visible = false
            }
            else if (title == "enabledrag") {
                appMouseArea.visible = true
            }
            else if (title.substring(0,1) == "o") {
                window.opacity = Number(title.substring(1,5))
            }
            else if (title == "enable_launcher" || title == "disable_launcher") {
                util.toggleLauncherCount(title)
            }
            else if (title.substring(0,1) == "t") {
                util.setLauncherCount(title.substring(1,title.length))
            }
            else if (title == "close") {
                window.close()
            }
            else if (title == "minimize") {
                window.visibility = Window.Minimized
            }
        }
    }

    MouseArea {
        id: appMouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        property point clickPos
        onPressed: {
            clickPos = Qt.point(mouse.x,mouse.y)
            cursorShape = Qt.ClosedHandCursor
        }
        onPositionChanged: {
            var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
            window.x = window.x + delta.x
            window.y = window.y + delta.y
        }
        onReleased: {
            if (window.x < 70 ) {
                window.x = 0
            }
            if (window.y < 70 ) {
                window.y = 0
            }
            util.saveWindowLocation(Qt.point(window.x, window.y));
            cursorShape = Qt.ArrowCursor
        }
    }
    onHeightChanged: {
        util.saveWindowSize(Qt.size(width, height))
    }

    onWidthChanged: {
        util.saveWindowSize(Qt.size(width, height))
    }
}
