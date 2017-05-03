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

Item {
    property string forecastDay
    property string forecastIcon
    property string forecastTemperature
    property string forecastDesc
    property string stringFont
    property string iconFont
    property string fontColor

    Rectangle {
        color: "transparent"
        anchors.fill: parent

        Text {
            id: date
            width: parent.width
            height: parent.height / 5
            anchors.top: parent.top
            anchors.left: parent.left
            color: fontColor
            font.family: stringFont
            font.pixelSize: 18
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: forecastDay
        }

        Text {
            id: weatherIcon
            width: parent.width
            height: parent.height / 5
            anchors.top: date.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: iconFont
            font.pixelSize: 20
            color: fontColor
            text: forecastIcon
        }

        Text {
            id: weatherTemperature
            anchors.top: weatherIcon.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            width: parent.width
            height: parent.height / 5
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: fontColor
            font.family: stringFont
            font.pixelSize: 14
            text: forecastTemperature
        }

        Text {
            id: weatherDescription
            anchors.top: weatherTemperature.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            width: parent.width
            height: parent.height / 5
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: fontColor
            font.family: stringFont
            font.pixelSize: 12
            wrapMode: Text.Wrap
            text: forecastDesc
        }
    }
}
