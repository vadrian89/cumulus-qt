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

Item {
    id: root
    property string fontFamily: "Arial"
    property string fontColor: "#ffffff"
    property string iconsFontFamily: ""
    property int fontSize: 20
    property alias viewModel: forecastList.model
    property int widthBreakPoint: 200

    ListView {
        id: forecastList
        anchors.fill: parent
        orientation: ListView.Vertical
        clip: true
        spacing: 2
        snapMode: ListView.SnapToItem
        delegate: ForecastItemDelegate {
            width: forecastList.width
            height: 60
            stringFont: root.fontFamily
            iconFont: root.iconsFontFamily
            fontColor: root.fontColor
            forecastDay: model.modelData.forecastDate
            forecastIcon: model.modelData.weatherIcon
            forecastTempHigh: model.modelData.tempHigh + "°"
            forecastTempLow: model.modelData.tempLow + "°"
            forecastDesc: model.modelData.forecastDesc
            radius: 2
            widthBreakPoint: root.widthBreakPoint
        }
        onModelChanged: positionViewAtBeginning()
        onWidthChanged: positionViewAtBeginning()
        onHeightChanged: positionViewAtBeginning()
    }
}
