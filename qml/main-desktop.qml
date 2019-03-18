/*
* Copyright (C) 2017 Adrian Verban <verbanady@gmail.com>
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
import QtQuick.Controls 1.4
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.2

import ownTypes.settingsController 1.0
import ownTypes.TrayController 0.3

Rectangle {
    id: mainWindow
    property int widthBreakPoint: 200
    color: applicationSettingsController.applicationBackground

    FontLoader {
        id: ubuntuCondensed
        source: "../assets/fonts/Ubuntu-C.ttf"
    }

    FontLoader {
        id: weatherIcons
        source: "../assets/fonts/weathericons-regular-webfont.ttf"
    }

    SettingsController {
        id: applicationSettingsController
        onWeatherApiChanged: bodyView.item.updateWeather()
        onFirstUseChanged: {
            if (!firstUse) {
                bodyView.source = "MainView.qml"
                bodyView.visible = true
                bodyView.item.updateWeather()
                applicationWindow.loadSettings()
            }
        }
    }

    TrayController {
        id: trayController
        trayVisibility: applicationSettingsController.trayVisibility
        trayTheme: applicationSettingsController.trayTheme
        onCloseApp: Qt.quit()
        onShowGui: applicationWindow.show()
    }

    ApplicationBar {
        id: applicationBar
        anchors.top: parent.top
        width: parent.width
        height: 30
        textColor: applicationSettingsController.textColor
        iconsFont: weatherIcons.name
        textFontFamily: ubuntuCondensed.name
        windowControlsPos: applicationSettingsController.windowControlsPos
        onRefreshButtonClicked: bodyView.item.updateWeather()
        onMenuButtonClicked: settingsViewDialog.visible = true        
        onCloseButtonClicked: applicationWindow.close()
        onMinimizeButtonClicked: applicationWindow.minimizeWindow()
        onLocationButtonClicked: bodyView.item.goToLocationsList()
        onMousePositionChanged: applicationWindow.moveWindow(mousePosition)
    }

    Loader {
        id: bodyView
        anchors.top: applicationBar.bottom
        width: parent.width
        height: parent.height - applicationBar.height
        visible: false
        source: ""
        Binding {
            target: bodyView.item
            property: "backgroundColor"
            value: applicationSettingsController.applicationBackground
        }
        Binding {
            target: bodyView.item
            property: "textColor"
            value: applicationSettingsController.textColor
        }
        Binding {
            target: bodyView.item
            property: "textFontFamily"
            value: ubuntuCondensed.name
        }
        Binding {
            target: bodyView.item
            property: "iconsFont"
            value: weatherIcons.name
        }
        Binding {
            target: bodyView.item
            property: "speedUnit"
            value: applicationSettingsController.windSpeedUnit
        }
        Binding {
            target: bodyView.item
            property: "widthBreakPoint"
            value: mainWindow.widthBreakPoint
        }
        Binding {
            target: bodyView.item
            property: "tempUnit"
            value: applicationSettingsController.tempUnit
        }
        Binding {
            target: bodyView.item
            property: "pressureUnit"
            value: applicationSettingsController.pressureUnit
        }
        Connections {
            id: weatherViewCon
            target: bodyView.item
            onTempValueChanged: trayController.temperature = bodyView.item.tempValue
            onFinishedWeatherUpdate: {
                applicationBar.animationAlias.stop()
                timer.interval = 3600000
            }
            onUpdateWeather: {
                if (applicationBar.animationAlias.running == false) {
                    applicationBar.animationAlias.start()
                    applicationBar.animationAlias.loops = RotationAnimation.Infinite
                }
            }
            onNetworkError: timer.interval = 60000
            onLocationNameChanged: applicationBar.locationName = name
        }
    }

    Timer {
        id: timer
        interval: 3600000
        running: true
        repeat: true
        onTriggered: bodyView.item.updateWeather()
    }

    Dialog {
        id: settingsViewDialog
        width: 300
        height: 500
        visible: false
        modality: Qt.ApplicationModal
        contentItem: SettingsWindow {
            id: settingsView
            anchors.fill: parent
            visible: true
            iconsFont: weatherIcons.name
            backgroundColor: applicationSettingsController.applicationBackground
            onBackgroundColorChanged: applicationSettingsController.applicationBackground = backgroundColor
            textColor: applicationSettingsController.textColor
            windowControls: applicationSettingsController.windowControlsPos
            onTextColorChanged: applicationSettingsController.textColor = textColor
            trayVisible: applicationSettingsController.trayVisibility
            onTrayVisibleChanged: applicationSettingsController.trayVisibility = trayVisible
            trayTheme: applicationSettingsController.trayTheme
            onTrayThemeChanged: applicationSettingsController.trayTheme = trayTheme
            onWindowControlsChanged: applicationSettingsController.windowControlsPos = windowControls
            onLoginStartChanged: applicationSettingsController.loginStart = loginStart
            onShowCredits: creditsViewDialog.visible = true
            api: applicationSettingsController.weatherApi
            onApiChanged: applicationSettingsController.weatherApi = api
            apiKey: applicationSettingsController.apiKey
            onApiKeyChanged: applicationSettingsController.apiKey = apiKey
            temperatureUnit: applicationSettingsController.tempUnit
            onTemperatureUnitChanged: applicationSettingsController.tempUnit = settingsView.temperatureUnit
            speedUnit: applicationSettingsController.windSpeedUnit
            onSpeedUnitChanged: applicationSettingsController.windSpeedUnit = settingsView.speedUnit
            onLocationChanged: {                
                settingsViewDialog.visible = false
                bodyView.item.updateWeather()
            }
            useGps: applicationSettingsController.useGps
            onUseGpsChanged: applicationSettingsController.useGps = useGps
            onCloseSettingsView: settingsViewDialog.visible = false
        }
        onVisibilityChanged: {
            if (visible == true)
                settingsView.forceActiveFocus()
            else
                bodyView.item.forceActiveFocus()
        }
    }

    Dialog {
        id: creditsViewDialog
        width: settingsView.width
        height: settingsView.height
        visible: false
        modality: Qt.ApplicationModal
        contentItem: CreditsView {
            id: creditsView
            anchors.fill: parent
            visible: true
            textColor: applicationSettingsController.textColor
            backgroundColor: applicationSettingsController.applicationBackground
            Keys.onEscapePressed: creditsViewDialog.visible = false
        }
        onVisibleChanged: {
            if (visible == true)
                creditsView.forceActiveFocus()
        }
    }
    WelcomeView {
        anchors.fill: parent
        color: applicationSettingsController.applicationBackground
        visible: applicationSettingsController.firstUse
        onFinished: applicationSettingsController.firstUse = false
    }
    Component.onCompleted: {
        if (!settingsController.firstUse) {
            bodyView.source = "MainView.qml"
            bodyView.visible = true
            bodyView.item.updateWeather()
            bodyView.item.forceActiveFocus()
        }
    }        
}
