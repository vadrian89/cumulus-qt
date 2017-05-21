/*
* Copyright (C) 2016 - 2017 Adrian Verban <verbanady@gmail.com>
* Maintainers: Adrian Verban <verbanady@gmail.com>
* Derived from Typhoon by Archisman Panigrahi which is based on Stormcloud by Jono Cooper <jonocooper.com>
* Thanks to all the contributors.
* Using the Ubuntu Condensed font.
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
import QtQuick 2.7
import QtQuick.Layouts 1.3

Item {
    id: root
    visible: true
    signal clicked()
    opacity: 0.6

    Rectangle {
        anchors.fill: parent
        color: "transparent"

        Image {
            anchors.centerIn: parent
            height: parent.height * 60 / 100
            width: parent.width * 60 / 100
            fillMode: Image.PreserveAspectFit
            source: "icons/settings-white.svg"
            sourceSize.width: width
            sourceSize.height: height
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            acceptedButtons: Qt.LeftButton
            onPressed: {
                cursorShape = Qt.ClosedHandCursor
                root.opacity = 1.0
            }
            onReleased: {
                cursorShape = Qt.PointingHandCursor
                root.opacity = 0.8
            }
            onClicked: {
                root.clicked()
            }
            onHoveredChanged: {
                if (containsMouse == true) {
                    root.opacity = 1.0
                }
                else {
                    root.opacity = 0.8
                }
            }
        }
    }
}
