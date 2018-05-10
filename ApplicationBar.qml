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
    signal menuButtonClicked()
    signal closeButtonClicked()
    signal minimizeButtonClicked()
    signal refreshButtonClicked()
    property string textColor
    property string iconsFont
    property alias animationAlias: refreshRotateAnim
    property alias moveControlAlias: moveControlBar
    property alias menuButtonAlias: menuButton
    property alias refreshButtonAlias: refreshButton
    property int controlButtonsSize: 18
    property string windowControlsPos: util.windowControlsPos()

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        Rectangle {
            id: closeButton
            anchors.top: parent.top
            anchors.left: windowControlsPos == "left" ? parent.left : minimizeButton.right
            height: parent.height
            width: height
            color: "#11000000"
            opacity: 0.8
            visible: false
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: root.textColor
                font.pixelSize: root.controlButtonsSize
                font.family: Qt.MonoOnly
                text: "x"
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onHoveredChanged: {
                    if (containsMouse == true) {
                        parent.opacity = 1.0
                        parent.color = "#33000000"
                    }
                    else {
                        parent.opacity = 0.8
                        parent.color = "#11000000"
                    }
                }
                onClicked: {
                    root.closeButtonClicked()
                }
            }
        }

        Rectangle {
            id: minimizeButton
            anchors.top: parent.top
            anchors.left: windowControlsPos == "left" ? closeButton.right : moveControlBar.right
            height: parent.height
            width: height
            color: "#11000000"
            opacity: 0.8
            visible: false
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: root.textColor
                font.pixelSize: root.controlButtonsSize
                font.family: Qt.MonoOnly
                text: "–"
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onHoveredChanged: {
                    if (containsMouse == true) {
                        parent.opacity = 1.0
                        parent.color = "#33000000"
                    }
                    else {
                        parent.opacity = 0.8
                        parent.color = "#11000000"
                    }
                }
                onClicked: {
                    root.minimizeButtonClicked()
                }
            }
        }

        MouseArea {
            id: moveControlBar
            anchors.top: parent.top
            anchors.left: windowControlsPos == "left" ? minimizeButton.right : refreshButton.right
            height: parent.height
            width: parent.width - (parent.height * 4)
        }

        Rectangle {
            id: refreshButton
            anchors.top: parent.top
            anchors.left: windowControlsPos == "left" ? moveControlBar.right : menuButton.right
            height: parent.height
            width: height
            color: "transparent"
            opacity: 0.8
            Image {
                anchors.centerIn: parent
                height: parent.height
                width: parent.width
                sourceSize.width: width
                sourceSize.height: height
                source: "image://fontimage/\uf021" + root.textColor
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onHoveredChanged: {
                    if (containsMouse == true) {
                        parent.opacity = 1.0
                    }
                    else {
                        parent.opacity = 0.8
                    }
                }
                onClicked: {
                    root.refreshButtonClicked()
                }
            }

            RotationAnimation on rotation {
                id: refreshRotateAnim
                direction: RotationAnimation.Clockwise
                from: 0
                to: 360
                loops: RotationAnimation.Infinite
                alwaysRunToEnd: true
            }
        }

        MenuButton {
            id: menuButton
            anchors.top: parent.top
            anchors.left: windowControlsPos == "left" ? refreshButton.right : parent.left
            height: parent.height
            width: height
            iconColor: root.textColor
            onClicked: root.menuButtonClicked()
        }
    }
}
