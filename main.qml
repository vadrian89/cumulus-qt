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
import QtQuick 2.5
import QtQuick.Window 2.2
import QtQml 2.2

import ownTypes.TrayController 0.1

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
    visible: util.applicationVisiblity() == false ? true : false

    Loader {
        id: viewLoader
        anchors.fill: parent
        source: "AppView.qml"
    }

    Connections {
        target: viewLoader.item
        onDisableDrag: appMouseArea.visible = false
        onEnableDrag: appMouseArea.visible = true
        onAppOpacityChanged: window.opacity = opacity
        onTrayIconChanged: trayController.icon = iconText
        onCloseApp: window.close()
        onHideApp: window.visible = false
        onMinimizeApp: window.visibility = Window.Minimized
        onChangeTrayVisibility: trayController.trayVisibility = util.applicationVisiblity()
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

    TrayController {
        id: trayController
        startUp: true
        trayVisibility: util.applicationVisiblity()

        onCloseApp: {
            Qt.quit()
        }

        onShowGui: {
            if(window.visible == false) {
                viewLoader.setSource("")
                viewLoader.setSource("AppView.qml")
            }
            window.visible = true
        }
    }
}
