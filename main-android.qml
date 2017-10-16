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
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

import ownTypes.settingsController 1.0

ApplicationWindow {
    id: mainWindow
    visible: true
    title: "Cumulus"

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

    Rectangle {
        id: appBody
        anchors.fill: parent
        color: applicationSettingsController.applicationBackground
        focus: true
        Keys.onBackPressed: mainWindow.close()

        ApplicationBar {
            id: applicationBar
            anchors.top: appBody.top
            width: appBody.width
            height: parent.height * 10 / 100
            textColor: applicationSettingsController.textColor
            iconsFont: weatherIcons.name
            onRefreshButtonClicked: weatherView.updateWeather()
            animationAlias.onStopped: weatherView.loadingEnded = true
            animationAlias.onStarted: weatherView.loadingEnded = false
            onMenuButtonClicked: {
                if (bodyView.visible == true) {
                    appView.push(settingsView)
                    settingsView.forceActiveFocus()
                }
                else {
                    appView.pop(null)
                    appBody.forceActiveFocus()
                }
            }
        }

        StackView {
            id: appView
            width: parent.width
            height: parent.height - applicationBar.height
            anchors.top: applicationBar.bottom
            initialItem: bodyView
        }

        Flickable {
            id: bodyView
            contentWidth: width
            contentHeight: height
            interactive: false
            contentX: 0
            WeatherWindow {
                id: weatherView
                width: bodyView.width
                height: bodyView.height
                textColor: applicationSettingsController.textColor
                textFontFamily: ubuntuCondensed.name
                iconsFont: weatherIcons.name
                speedUnit: settingsView.speedUnit
                visible: false
                onNoLocationDetected: {
                    appView.push(settingsView)
                    creditsView.forceActiveFocus()
                    settingsView.searchLocationAlias.visible = true
                    applicationBar.menuButtonAlias.visible = false
                    applicationBar.refreshButtonAlias.visible = false
                }
                onFinishedWeatherUpdate: {
                    visible = true
                    applicationBar.menuButtonAlias.visible = true
                    applicationBar.refreshButtonAlias.visible = true
                }
                onUpdateWeather: {
                    if (applicationBar.animationAlias.running == false) {
                        applicationBar.animationAlias.start()
                        applicationBar.animationAlias.loops = RotationAnimation.Infinite
                    }
                }
                onDataDownloadFinished: {
                    applicationBar.animationAlias.stop()
                    timer.interval = 3600000
                }
            }
            Component.onCompleted: weatherView.updateWeather()
        }

        SettingsWindow {
            id: settingsView
            onLocationChanged: {
                weatherView.updateWeather()
                appView.pop()
                bodyView.forceActiveFocus()
            }
            onTextColorChanged: applicationSettingsController.textColor = textColor
            onBackgroundColorChanged: applicationSettingsController.applicationBackground = backgroundColor
            onTemperatureUnitChanged: {
                if (visible == true) {
                    weatherView.changeTempUnit(settingsView.temperatureUnit)
                }
            }
            onSpeedUnitChanged: {
                if (visible == true) {
                    weatherView.changeSpeedUnit(settingsView.speedUnit)
                }
            }
            onApiChanged: {
                if (visible == true) {
                    weatherView.weatherApi = api
                }
            }
            onShowCredits: {
                appView.push(creditsView)
                creditsView.forceActiveFocus()
            }
            Keys.onBackPressed: {
                appView.pop()
                bodyView.forceActiveFocus()
            }
        }

        CreditsView {
            id: creditsView
            textColor: applicationSettingsController.textColor
            backgroundColor: applicationSettingsController.applicationBackground
            Keys.onBackPressed: {
                appView.pop()
                settingsView.forceActiveFocus()
            }
        }
    }

    Timer {
        id: timer
        interval: 3600000
        running: true
        repeat: true
        onTriggered: weatherView.updateWeather()
    }
}
