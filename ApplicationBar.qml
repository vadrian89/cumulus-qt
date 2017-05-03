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

    Rectangle {
        anchors.fill: parent
        color: "transparent"

        RowLayout {
            anchors.fill: parent
            spacing: 0

            Row {
                id: windowControl
                Layout.fillWidth: false
                Layout.fillHeight: true
                Layout.preferredWidth: height * 2
                Layout.alignment: Qt.AlignLeft
                spacing: 0
                visible: util.osType() === "android" ? false : true
                Rectangle {
                    id: closeButton
                    height: parent.height
                    width: height
                    color: "#11000000"
                    opacity: 0.8
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
                        cursorShape: Qt.PointingHandCursor
                        onHoveredChanged: {
                            if ( containsMouse == true ) {
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
                    height: parent.height
                    width: height
                    color: "#11000000"
                    opacity: 0.8
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
                        cursorShape: Qt.PointingHandCursor
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
            }

            MouseArea {
                id: moveControlBar
                Layout.fillWidth: true
                Layout.fillHeight: true
                cursorShape: Qt.PointingHandCursor
            }

            Rectangle {
                id: refreshButton
                height: parent.height
                width: height
                color: "transparent"
                opacity: 0.8
                Layout.fillWidth: false
                Layout.fillHeight: true
                Layout.preferredWidth: height
                Layout.alignment: Qt.AlignRight
                anchors.right: menuButton.left
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: parent.height * 70 / 100
                    font.family: iconsFont
                    text: "\uf04c"
                    color: root.textColor                    
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
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
                Layout.fillWidth: false
                Layout.fillHeight: true
                Layout.preferredWidth: height
                Layout.alignment: Qt.AlignRight
                onClicked: root.menuButtonClicked()
            }
        }
    }
}
