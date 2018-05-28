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
import QtQuick 2.0
import QtQuick.Layouts 1.0

Item {
    id: root
    signal clicked()
    signal pressed()
    signal released()
    property alias iconUrl: optionIcon.source
    property alias optionText: optionText.text
    property alias backgroundColor: rectangle.color
    property alias textColor: optionText.color
    property alias bottomBorderVisibility: bottomBorder.visible

    Rectangle {
        id: rectangle
        anchors.fill: parent

        RowLayout {
            anchors.fill: parent

            Image {
                id: optionIcon
                Layout.fillHeight: false
                Layout.fillWidth: false
                Layout.preferredHeight: parent.height * 50 / 100
                Layout.preferredWidth: height
                Layout.alignment: Qt.AlignVCenter
                fillMode: Image.PreserveAspectFit
                anchors.leftMargin: (parent.width - (parent.width * 80 / 100)) / 2
                anchors.left: parent.left
            }
            Text {
                id: optionText
                Layout.fillHeight: false
                Layout.fillWidth: false
                Layout.preferredWidth: parent.width - optionIcon.width - 9
                Layout.alignment: Qt.AlignVCenter
                anchors.leftMargin: 20
                anchors.left: optionIcon.right
                font.pixelSize: 18
                color: "#555656"
            }
        }
        Rectangle {
            id: bottomBorder
            color: "#666C6C6C"
            height: 1
            width: parent.width * 80 / 100
            anchors.bottom: rectangle.bottom
            anchors.left: parent.left
            anchors.leftMargin: (parent.width - width) / 2
            visible: true
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                root.clicked()
            }
            onPressed: {
                root.pressed()
                parent.color = "#DDDDDD"
            }
            onReleased: {
                root.released()
                parent.color = "#FFFFFF"
            }
            onExited: parent.color = "#FFFFFF"
        }
    }    
}
