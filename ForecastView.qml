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
    property string fontFamily
    property string fontColor
    property int fontSize
    property alias viewModel: forecastList.model
    property int widthBreakPoint: 170
    property int delegateWidth: root.width >= widthBreakPoint ? root.width / 5 : root.width / 2

    FontLoader {
        id: weatherIconsFont
        source: "fonts/weathericons-regular-webfont.ttf"
    }

    ListView {
        id: forecastList
        anchors.fill: parent
        orientation: ListView.Horizontal
        clip: true
        spacing: 0
        snapMode: ListView.SnapToItem
        delegate: ForecastDelegate {
            height: forecastList.height
            width: delegateWidth
            stringFont: fontFamily
            iconFont: weatherIconsFont.name
            fontColor: root.fontColor
            forecastDay: model.modelData.forecastDate
            forecastIcon: model.modelData.weatherCode
            forecastTemperature: model.modelData.tempHigh + "° / " + model.modelData.tempLow + "°"
            forecastDesc: model.modelData.forecastDesc
        }
        onModelChanged: positionViewAtBeginning()
        onWidthChanged: positionViewAtBeginning()
        onHeightChanged: positionViewAtBeginning()
    }
}
