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
    id: root
    property string icon
    property string infoText
    property string iconFont
    property int fontSize
    property string fontFamily
    property string fontColor
    property alias iconRotation: dataIcon.rotation
    property bool fontBold: false

    Text {
        id: dataIcon
        anchors.left: root.left
        anchors.top: root.top
        width: root.width * 20 / 100
        height: root.height
        font.pixelSize: fontSize
        font.family: iconFont
        font.bold: fontBold
        color: fontColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: icon        
    }
    Text {
        id: dataText
        anchors.left: dataIcon.right
        anchors.leftMargin: 10
        anchors.top: root.top
        width: root.width - dataIcon.width
        height: root.height
        font.pixelSize: fontSize
        font.family: fontFamily
        font.bold: fontBold
        color: fontColor
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        text: infoText
    }
}
