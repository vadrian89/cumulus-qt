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
    property string text
    property string textColor
    property string backgroundColor
    property string buttonBorderColor: "#FFFFFF"
    signal clicked()

    RowLayout {
        anchors.fill: parent
        spacing: 10
        Text {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: root.textColor
            text: root.text
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        Rectangle {
            Layout.fillHeight: false
            Layout.fillWidth: false
            Layout.preferredHeight: 30
            Layout.preferredWidth: 30
            color: root.backgroundColor
            radius: 15
            border.width: 2
            border.color: root.buttonBorderColor
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: root.clicked()
            }
        }
    }
}
