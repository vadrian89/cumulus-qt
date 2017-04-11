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
import QtQuick 2.5
import QtQuick.Layouts 1.1

import ownTypes.weather 1.8

Item {
    id: root
    property alias loadingEnded: weatherController.loadFinished
    property alias tempValue: weatherController.temperature
    property alias tempUnit: weatherController.tempUnit
    property alias speedUnitSymbol: weatherController.speedUnit
    property string iconsFont
    property string textFontFamily
    property string textColor    
    property string speedUnit: util.getWindSpeedUnit()    
    property alias weatherApi: weatherController.weatherApi
    property int widthBreakPoint: 170
    property int locationHeight: Math.round(root.height * 10 / 100)
    property int weatherInfoHeight: Math.round(root.height * 50 / 100)
    property int weatherIconHeight: Math.round(root.height * 35 / 100)
    property int weatherTempHeight: Math.round(root.height * 15 / 100)
    property int forecastHeight: Math.round(root.height * 25 / 100)
    property int logoHeight: Math.round(root.height * 15 / 100)
    property int tempFontSize: root.height >= 380 ? 40 : 25
    signal updateWeather()
    signal finishedWeatherUpdate()
    signal startedWeatherUpdate()
    signal noLocationDetected()
    signal dataDownloadFinished()
    signal changeTempUnit(string unit)
    signal changeSpeedUnit(string unit)

    Text {
        id: locationText
        width: parent.width
        height: locationHeight
        color: textColor
        anchors.top: parent.top
        anchors.left: parent.left
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text: weatherController.location + ": " + util.firstLetterUp(weatherController.weatherDescription)
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
        anchors.top: locationText.visible == true ? locationText.bottom : root.top
        anchors.left: parent.left
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.family: iconsFont
        font.pixelSize: root.height >= 170 ? 80 : 60
        color: textColor
        text: weatherController.weatherIcon
    }

    Rectangle {
        id: weatherInfo
        anchors.top: locationText.visible == true ? locationText.bottom : root.top
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
            fontColor: textColor
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
            fontColor: textColor
            fontFamily: textFontFamily
            iconFont: iconsFont
            icon: "\uf0b1"
            infoText: Math.round(weatherController.windSpeed) + root.speedUnitSymbol
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
            fontColor: textColor
            fontFamily: textFontFamily
            iconFont: iconsFont
            icon: "\uf079"
            infoText: (Math.round(weatherController.pressure * 10) / 10) + util.pressureUnitSymbol()
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
            fontColor: textColor
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
            fontColor: textColor
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
            fontColor: textColor
            fontFamily: textFontFamily
            iconFont: iconsFont
            icon: "\uf058"
            infoText: weatherController.tempMax + root.tempUnit
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
            fontColor: textColor
            fontFamily: textFontFamily
            iconFont: iconsFont
            icon: "\uf044"
            infoText: weatherController.tempMin + root.tempUnit
        }
    }

    Text {
        id: weatherTemperatureIcon
        width: weatherIcon.width * 35 / 100
        height: weatherTempHeight
        color: textColor
        anchors.top: weatherIcon.bottom
        anchors.left: parent.left
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        font.pixelSize: tempFontSize
        font.family: iconsFont
        font.bold: true
        text: "\uf055"
    }

    Text {
        id: weatherTemperature
        width: (weatherIcon.width * 65 / 100) - 10
        height: weatherTempHeight
        color: textColor
        anchors.top: weatherIcon.bottom
        anchors.left: weatherTemperatureIcon.right
        anchors.leftMargin: 10
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: weatherTemperatureIcon.font.pixelSize
        font.family: textFontFamily
        font.bold: true
        text: weatherController.temperature + root.tempUnit
    }

    ForecastView {
        id: forecastView
        width: root.width - 20
        height: forecastHeight
        anchors.top: weatherTemperature.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        fontColor: textColor
        fontFamily: textFontFamily
        viewModel: weatherController.forecastList
        widthBreakPoint: root.widthBreakPoint
    }

    ColumnLayout {
        id: logoLayout
        anchors.top: forecastView.visible == true ? forecastView.bottom : weatherTemperatureIcon.bottom
        anchors.left: parent.left
        height: logoHeight
        width: parent.width
        Image {
            id: logo
            Layout.fillHeight: true
            Layout.fillWidth: false
            Layout.preferredWidth: root.width > root.widthBreakPoint ? logoLayout.width * 60 / 100 : logoLayout.width
            Layout.alignment: Qt.AlignCenter
            fillMode: Image.PreserveAspectFit
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            source: util.getLogoImage()
        }
    }

    Weather {
        id: weatherController
        onNoLocationSet: root.noLocationDetected()
        onDataDownloadFinished: root.dataDownloadFinished()
        onWeatherApiChanged: updateWeather()
        onWeatherDataChanged: {
            weatherController.getForecastData()
            root.finishedWeatherUpdate()
            logo.source = util.getLogoImage()
        }        
    }
    onUpdateWeather: {
        console.log("WeatherWindow >> update started")
        weatherController.getWeatherData()
    }
    onChangeTempUnit: weatherController.changeTempUnit(unit)
    onChangeSpeedUnit: weatherController.changeSpeedUnit(unit)

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
            locationText.visible = false
            logoLayout.visible = false
            forecastView.visible = false
            weatherIconHeight = Math.round(root.height * 80 / 100)
            weatherInfoHeight = root.height
        }
        else if (height >= 150 && height < 170) {
            locationText.visible = true
            logoLayout.visible = false
            forecastView.visible = false
            weatherIconHeight = Math.round(root.height * 70 / 100)
            weatherInfoHeight = Math.round(root.height * 90 / 100)
        }
        else if (height >= 170 && height < 300) {
            locationText.visible = true
            logoLayout.visible = true
            forecastView.visible = false
            weatherIconHeight = Math.round(root.height * 50 / 100)
            weatherInfoHeight = Math.round(root.height * 65 / 100)
            logoHeight = Math.round(root.height * 25 / 100)
        }
        else {
            locationText.visible = true
            logoLayout.visible = true
            forecastView.visible = true
            weatherIconHeight = Math.round(root.height * 35 / 100)
            weatherInfoHeight = Math.round(root.height * 50 / 100)
            logoHeight = Math.round(root.height * 15 / 100)
        }
    }
}
