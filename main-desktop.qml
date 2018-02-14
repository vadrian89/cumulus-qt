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
import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

import ownTypes.settingsController 1.0
import ownTypes.TrayController 0.3

Rectangle {
    id: mainWindow
    property int widthBreakPoint: 170
    color: applicationSettingsController.applicationBackground

    FontLoader {
        id: ubuntuCondensed
        source: "fonts/Ubuntu-C.ttf"
    }

    FontLoader {
        id: weatherIcons
        source: "fonts/weathericons-regular-webfont.ttf"
    }

    SettingsController {
        id: applicationSettingsController
    }

    ApplicationBar {
        id: applicationBar
        anchors.top: parent.top
        width: parent.width
        height: 30
        textColor: applicationSettingsController.textColor
        iconsFont: weatherIcons.name
        windowControlsPos: applicationSettingsController.windowControlsPos
        onRefreshButtonClicked: weatherViewLoader.item.updateWeather()
        onMenuButtonClicked: settingsViewDialog.visible = true
    }

    Flickable {
        id: bodyView
        anchors.top: applicationBar.bottom
        width: parent.width
        height: parent.height - applicationBar.height
        contentHeight: parent.height
        contentWidth: parent.width
        contentX: 0
        interactive: false
        Loader {
            id: weatherViewLoader
            width: bodyView.width
            height: bodyView.height
            visible: false
            Binding {
                target: weatherViewLoader.item
                property: "textColor"
                value: applicationSettingsController.textColor
            }
            Binding {
                target: weatherViewLoader.item
                property: "textFontFamily"
                value: ubuntuCondensed.name
            }
            Binding {
                target: weatherViewLoader.item
                property: "iconsFont"
                value: weatherIcons.name
            }
            Binding {
                target: weatherViewLoader.item
                property: "speedUnit"
                value: settingsView.speedUnit
            }
            Binding {
                target: weatherViewLoader.item
                property: "widthBreakPoint"
                value: mainWindow.widthBreakPoint
            }
            Binding {
                target: weatherViewLoader.item
                property: "loadingEnded"
                value: !applicationBar.animationAlias.running
            }
            Connections {
                id: weatherViewCon
                target: weatherViewLoader.item
                onNoLocationDetected: {
                    settingsViewDialog.visible = true
                    settingsView.searchLocationAlias.visible = true
                    applicationBar.menuButtonAlias.visible = false
                    applicationBar.refreshButtonAlias.visible = false
                }
                onFinishedWeatherUpdate: {
                    weatherViewLoader.visible = true
                    applicationBar.menuButtonAlias.visible = true
                    applicationBar.refreshButtonAlias.visible = true
                    applicationBar.animationAlias.stop()
                    timer.interval = 3600000
                }
                onUpdateWeather: {
                    if (applicationBar.animationAlias.running == false) {
                        applicationBar.animationAlias.start()
                        applicationBar.animationAlias.loops = RotationAnimation.Infinite
                    }
                }
                onNetworkError: {
                    timer.interval = 60000
                }
            }
        }
    }
    Component.onCompleted: {
        weatherViewLoader.source = "WeatherWindow.qml"
        weatherViewLoader.item.updateWeather()
    }


    TrayController {
        id: trayController
        trayVisibility: applicationSettingsController.trayVisibility
        trayTheme: applicationSettingsController.trayTheme
        icon: weatherViewLoader.item.tempValue
        onCloseApp: Qt.quit()
        onShowGui: applicationWindow.show()
    }

    Timer {
        id: timer
        interval: 3600000
        running: true
        repeat: true
        onTriggered: weatherViewLoader.item.updateWeather()
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
            onTrayVisibleChanged: applicationSettingsController.trayVisibility = trayVisible
            onTrayThemeChanged: applicationSettingsController.trayTheme = trayTheme
            onWindowControlsChanged: applicationSettingsController.windowControlsPos = windowControls
            onLoginStartChanged: applicationSettingsController.loginStart = loginStart
            onBackgroundColorChanged: applicationSettingsController.applicationBackground = backgroundColor
            onShowCredits: creditsViewDialog.visible = true
            onLocationChanged: {
                weatherViewLoader.item.updateWeather()
                settingsViewDialog.visible = false
            }
            onTextColorChanged: {
                applicationSettingsController.textColor = textColor
                weatherViewLoader.item.loadLogoImage()
            }
            onTemperatureUnitChanged: {
                if (settingsViewDialog.visible == true) {
                    weatherViewLoader.item.changeTempUnit(settingsView.temperatureUnit)
                }
            }
            onSpeedUnitChanged: {
                if (settingsViewDialog.visible == true) {
                    weatherViewLoader.item.changeSpeedUnit(settingsView.speedUnit)
                }
            }
            onApiChanged: {
                if (settingsViewDialog.visible == true) {
                    weatherViewLoader.item.weatherApi = api
                }
            }
            Keys.onEscapePressed: settingsViewDialog.visible = false
        }
        onVisibilityChanged: {
            if (visible == true)
                settingsView.forceActiveFocus()
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
}
