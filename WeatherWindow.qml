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
import QtQuick.Layouts 1.3

import ownTypes.weather 1.9

Item {
    id: root
    visible: false
    property alias tempValue: weatherController.temperature
    property string tempUnit
    property string iconsFont: ""
    property string textFontFamily
    property string textColor    
    property string speedUnit
    property string pressureUnit
    property alias locationName: weatherController.location
    property int widthBreakPoint: 170
    property int descriptionHeight: 30
    property int weatherInfoHeight: 180
    property int weatherIconHeight: 130
    property int weatherTempHeight: 50
    property int forecastHeight: Math.round(root.height - descriptionHeight - weatherIconHeight - weatherTempHeight - logo.height - logo.anchors.bottomMargin - 5)
    property int tempFontSize: 25
    signal updateWeather()
    signal setWeather()
    signal finishedWeatherUpdate()
    signal noLocationDetected()
    signal networkError()
    signal loadLogoImage()    
    onLoadLogoImage: logo.source = util.getLogoImage()
    onSetWeather: weatherController.setWeatherData()
    Text {
        id: descriptionText
        width: parent.width
        height: descriptionHeight
        color: root.textColor
        anchors.top: parent.top
        anchors.left: parent.left
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text: weatherController.weatherDescription
        font.family: textFontFamily
        font.pixelSize: 20
        wrapMode: Text.WordWrap
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: Qt.openUrlExternally(weatherController.locationLink)
        }
    }

    Text {
        id: weatherIcon
        width: parent.width * 50 / 100
        height: weatherIconHeight
        anchors.top: descriptionText.visible == true ? descriptionText.bottom : root.top
        anchors.left: parent.left
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.family: iconsFont
        font.pixelSize: (height * 0.6)
        color: root.textColor
        text: weatherController.weatherIcon
    }

    Rectangle {
        id: weatherInfo
        anchors.top: descriptionText.visible == true ? descriptionText.bottom : root.top
        anchors.left: weatherIcon.right
        width: parent.width - weatherIcon.width
        height: weatherInfoHeight
        color: "transparent"
        property int weatherInfoFontSize: 16

        WeatherInfoItem {
            id: humidityData
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            width: parent.width - 5
            height: parent.height * 8 / 100
            fontSize: weatherInfo.weatherInfoFontSize
            fontColor: root.textColor
            fontFamily: textFontFamily
            iconFont: iconsFont
            icon: "\uf07a"
            infoText: weatherController.humidity + " %"
        }
        WeatherInfoItem {
            id: windData
            anchors.top: humidityData.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 10
            width: parent.width - 5
            height: parent.height * 8 / 100
            fontSize: weatherInfo.weatherInfoFontSize
            fontColor: root.textColor
            fontFamily: textFontFamily
            iconFont: iconsFont
            icon: "\uf0b1"
            infoText: Math.round(weatherController.windSpeed) + util.speedUnitSymbol(root.speedUnit)
            iconRotation: weatherController.windDegree
        }
        WeatherInfoItem {
            id: pressureData
            anchors.top: windData.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 10
            width: parent.width - 5
            height: parent.height * 8 / 100
            fontSize: weatherInfo.weatherInfoFontSize
            fontColor: root.textColor
            fontFamily: textFontFamily
            iconFont: iconsFont
            icon: "\uf079"
            infoText: (Math.round(weatherController.pressure * 10) / 10) + util.pressureUnitSymbol(root.pressureUnit)
        }
        WeatherInfoItem {
            id: sunriseData
            anchors.top: pressureData.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 10
            width: parent.width - 5
            height: parent.height * 8 / 100
            fontSize: weatherInfo.weatherInfoFontSize
            fontColor: root.textColor
            fontFamily: textFontFamily
            iconFont: iconsFont
            icon: "\uf051"
            infoText: weatherController.sunrise
        }
        WeatherInfoItem {
            id: sunsetData
            anchors.top: sunriseData.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 10
            width: parent.width - 5
            height: parent.height * 8 / 100
            fontSize: weatherInfo.weatherInfoFontSize
            fontColor: root.textColor
            fontFamily: textFontFamily
            iconFont: iconsFont
            icon: "\uf052"
            infoText: weatherController.sunset
        }
        WeatherInfoItem {
            id: tempMax
            anchors.top: sunsetData.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 10
            width: parent.width - 5
            height: parent.height * 8 / 100
            fontSize: weatherInfo.weatherInfoFontSize
            fontColor: root.textColor
            fontFamily: textFontFamily
            iconFont: iconsFont
            icon: "\uf058"
            infoText: weatherController.tempMax + util.tempUnitSymbol(root.tempUnit)
        }
        WeatherInfoItem {
            id: tempMin
            anchors.top: tempMax.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 10
            width: parent.width - 5
            height: parent.height * 8 / 100
            fontSize: weatherInfo.weatherInfoFontSize
            fontColor: root.textColor
            fontFamily: textFontFamily
            iconFont: iconsFont
            icon: "\uf044"
            infoText: weatherController.tempMin + util.tempUnitSymbol(root.tempUnit)
        }
    }
    Text {
        id: weatherTemperatureIcon
        width: weatherIcon.width * 35 / 100
        height: weatherTempHeight
        color: root.textColor
        anchors.top: weatherIcon.bottom
        anchors.left: parent.left
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        font.pixelSize: (height * 0.6)
        font.family: iconsFont
        font.bold: true
        text: "\uf055"
    }
    Text {
        id: weatherTemperature
        width: (weatherIcon.width * 65 / 100) - 10
        height: weatherTempHeight
        color: root.textColor
        anchors.top: weatherIcon.bottom
        anchors.left: weatherTemperatureIcon.right
        anchors.leftMargin: 10
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: weatherTemperatureIcon.font.pixelSize
        font.family: textFontFamily
        font.bold: true
        text: weatherController.temperature + util.tempUnitSymbol(root.tempUnit)
    }
    ForecastView {
        id: forecastView
        width: root.width - 20
        height: forecastHeight
        anchors.top: weatherTemperature.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        fontColor: root.textColor
        fontFamily: root.textFontFamily
        iconsFontFamily: root.iconsFont
        viewModel: weatherController.forecastList
    }
    Image {
        id: logo
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: ((parent.width / 2) - (logo.width / 2))
        width: 170
        fillMode: Image.PreserveAspectFit
        horizontalAlignment: Image.AlignHCenter
        verticalAlignment: Image.AlignVCenter
        source: util.getLogoImage()
    }
    Weather {
        id: weatherController
        onNetworkError: {
            root.networkError()
            setWeatherData()
        }
        tempUnit: root.tempUnit
        speedUnit: root.speedUnit
        onWeatherDataChanged: {
            root.visible = true
            root.finishedWeatherUpdate()
            logo.source = util.getLogoImage()
        }
    }
    onUpdateWeather: weatherController.getWeatherData()
    onWidthChanged: {
        if (width < widthBreakPoint) {
            weatherIcon.width = root.width
            weatherInfo.visible = false
        }
        else if (width >= widthBreakPoint){
            weatherIcon.width = root.width * 50 / 100
            weatherInfo.visible = true
        }
    }
    onHeightChanged: {
        if (height < 150) {
            descriptionText.visible = false
            logo.visible = false
            forecastView.visible = false
        }
        else if (height >= 150 && height < 170) {
            descriptionText.visible = true
            logo.visible = false
            forecastView.visible = false
        }
        else if (height >= 170 && height < 300) {
            descriptionText.visible = true
            logo.visible = true
            forecastView.visible = false
        }
        else {
            descriptionText.visible = true
            logo.visible = true
            forecastView.visible = true
        }
    }
}
