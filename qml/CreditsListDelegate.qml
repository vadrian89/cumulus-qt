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
    property string author
    property string authorUrl
    property string license
    property string icon
    property string textColor
    visible: true
    Rectangle {
        anchors.fill: parent
        color: "transparent"
        RowLayout {
            anchors.fill: parent
            Rectangle {
                Layout.fillHeight: false
                Layout.fillWidth: false
                Layout.preferredHeight: parent.height
                Layout.preferredWidth: parent.height
                Layout.alignment: Qt.AlignCenter
                color: "transparent"
                Image {
                    anchors.centerIn: parent
                    width: 28
                    height: 28
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: util.iconPathPrefix() + root.icon
                }
            }
            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                Text {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignBottom
                    text: "Author: " + root.author
                    color: root.textColor
                }
                Text {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    text: "License: " + root.license
                    color: root.textColor
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: Qt.openUrlExternally(authorUrl)
            cursorShape: Qt.PointingHandCursor
        }
    }
    Rectangle {
        id: bottomBorder
        color: root.textColor
        height: 1
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: (parent.width - width) / 2
        visible: true
    }    
}
