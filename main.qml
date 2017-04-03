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
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

import ownTypes.settingsController 0.5
import ownTypes.TrayController 0.1

ApplicationWindow {
    id: mainWindow
    property point mainWindowLocation: util.loadWindowLocation()
    property size mainWindowSize: util.loadWindowSize()
    property int winMinHeight: 150
    property int winMinWidth: 140
    property int widthBreakPoint: 170
    visible: util.trayVisibility() == true ? false : true
    minimumHeight: winMinHeight
    minimumWidth: winMinWidth
    height: mainWindowSize.height
    width: mainWindowSize.width
    x: mainWindowLocation.x
    y: mainWindowLocation.y
    title: qsTr("Cumulus")
    color: "transparent"
    flags: Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint

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
        onApplicationOpacityChanged: mainWindow.opacity = applicationOpacity
    }

    onClosing: {
        util.saveWindowSize(Qt.size(width, height))
        if (applicationSettingsController.trayVisibility == true) {
            visible = false
        }
        else {
            Qt.quit()
        }
    }

    Rectangle {
        id: appBody
        anchors.fill: parent
        color: applicationSettingsController.applicationBackground

        ApplicationBar {
            id: applicationBar
            anchors.top: appBody.top
            width: appBody.width
            height: 30
            textColor: applicationSettingsController.textColor
            iconsFont: weatherIcons.name
            onMenuButtonClicked: {
                settingsView.visible = true
            }
            onCloseButtonClicked: mainWindow.close()
            onMinimizeButtonClicked: mainWindow.visibility = Window.Minimized
            onRefreshButtonClicked: {
                weatherView.updateWeather()
            }
            moveControlAlias.acceptedButtons: Qt.LeftButton
            property point clickPos
            moveControlAlias.onPressed: {
                clickPos = Qt.point(mouse.x,mouse.y)
                moveControlAlias.cursorShape = Qt.ClosedHandCursor
                mainWindow.maximumHeight = mainWindow.height
                mainWindow.maximumWidth = mainWindow.width
                mainWindow.minimumHeight = mainWindow.height
                mainWindow.minimumWidth = mainWindow.width
            }
            moveControlAlias.onPositionChanged: {
                var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                mainWindow.x = mainWindow.x + delta.x
                mainWindow.y = mainWindow.y + delta.y
            }
            moveControlAlias.onReleased: {
                if (mainWindow.x < 70 ) {
                    mainWindow.x = 0
                }
                if (mainWindow.y < 70 ) {
                    mainWindow.y = 0
                }
                util.saveWindowLocation(Qt.point(mainWindow.x, mainWindow.y));
                moveControlAlias.cursorShape = Qt.ArrowCursor
                mainWindow.maximumHeight = Screen.desktopAvailableHeight
                mainWindow.maximumWidth = Screen.desktopAvailableWidth
                mainWindow.minimumHeight = winMinHeight
                mainWindow.minimumWidth = winMinWidth
            }
            animationAlias.onStopped: {
                weatherView.loadingEnded = true
            }
            animationAlias.onStarted: {
                weatherView.loadingEnded = false
            }
        }

        Flickable {
            id: bodyView
            anchors.top: applicationBar.bottom
            width: appBody.width
            height: appBody.height - applicationBar.height
            contentHeight: height
            contentWidth: width
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
                widthBreakPoint: mainWindow.widthBreakPoint
                Keys.onBackPressed: {
                    Qt.quit()
                }
                onNoLocationDetected: {
                    visible = false
                    settingsView.visible = true
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
        }
        Timer {
            id: timer
            interval: 3600000
            running: true
            repeat: true
            onTriggered: weatherView.updateWeather()
        }
    }

    SettingsWindow {
        id: settingsView
        width: mainWindow.width >= 300 ? mainWindow.width : 300
        height: mainWindow.height >= 500 ? mainWindow.height : 500
        onTrayVisibleChanged: applicationSettingsController.trayVisibility = trayVisible
        onLocationChanged: {
            weatherView.updateWeather()
            visible = false
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
    }
    TrayController {
        id: trayController
        startUp: true        
        trayVisibility: applicationSettingsController.trayVisibility
        icon: weatherView.tempValue
        onCloseApp: Qt.quit()
        onShowGui: mainWindow.visible = true
    }
}
