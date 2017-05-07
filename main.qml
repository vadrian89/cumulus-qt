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
import QtQuick.Window 2.2

import ownTypes.settingsController 0.6
import ownTypes.TrayController 0.3

ApplicationWindow {
    id: mainWindow
    property point mainWindowLocation: util.loadWindowLocation()
    property size mainWindowSize: util.loadWindowSize()
    property int winMinHeight: 150
    property int winMinWidth: 140
    property int widthBreakPoint: 170
    property int minimumX: (Screen.width - Screen.desktopAvailableWidth)
    property int minimumY: (Screen.height - Screen.desktopAvailableHeight)
    visible: util.trayVisibility() == true ? false : true
    minimumHeight: winMinHeight
    minimumWidth: winMinWidth
    height: mainWindowSize.height
    width: mainWindowSize.width
    x: mainWindowLocation.x
    y: mainWindowLocation.y
    title: qsTr("Cumulus")
    color: "transparent"
    flags: Qt.CustomizeWindowHint

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
        util.saveWindowSize(Qt.size(mainWindow.width, mainWindow.height))
        if (applicationSettingsController.trayVisibility == true) {
            visible = false
        }
        else {
            Qt.quit()
        }
    }

    background: Rectangle {
        color: "transparent"
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
            windowControlsPos: applicationSettingsController.windowControlsPos
            onMenuButtonClicked: settingsViewDialog.visible = true
            onCloseButtonClicked: mainWindow.close()
            onMinimizeButtonClicked: mainWindow.visibility = Window.Minimized
            onRefreshButtonClicked: weatherView.updateWeather()
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
                if (mainWindow.x <= minimumX ) {
                    mainWindow.x = 0
                }
                if (mainWindow.y <= minimumY ) {
                    mainWindow.y = 0
                }
                util.saveWindowLocation(Qt.point(mainWindow.x, mainWindow.y))
                moveControlAlias.cursorShape = Qt.ArrowCursor
                mainWindow.maximumHeight = Screen.desktopAvailableHeight
                mainWindow.maximumWidth = Screen.desktopAvailableWidth
                mainWindow.minimumHeight = winMinHeight
                mainWindow.minimumWidth = winMinWidth
            }
            animationAlias.onStopped: weatherView.loadingEnded = true
            animationAlias.onStarted: weatherView.loadingEnded = false
        }

        Flickable {
            id: bodyView
            anchors.top: applicationBar.bottom
            width: parent.width
            height: parent.height - applicationBar.height
            contentHeight: bodyView.height
            contentWidth: bodyView.width
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
                widthBreakPoint: mainWindow.widthBreakPoint
                visible: false
                onNoLocationDetected: {
                    settingsViewDialog.visible = true
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
    }
    Component.onCompleted: weatherView.updateWeather()

    ApplicationWindow {
        id: settingsViewDialog
        width: 300
        height: 500
        visible: false
        flags: Qt.Dialog
        SettingsWindow {
            id: settingsView
            anchors.fill: parent
            visible: true
            onTrayVisibleChanged: applicationSettingsController.trayVisibility = trayVisible
            onTrayThemeChanged: applicationSettingsController.trayTheme = trayTheme
            onWindowControlsChanged: applicationSettingsController.windowControlsPos = windowControls
            onLocationChanged: {
                weatherView.updateWeather()
                settingsViewDialog.visible = false
            }
            onTextColorChanged: applicationSettingsController.textColor = textColor
            onBackgroundColorChanged: applicationSettingsController.applicationBackground = backgroundColor
            onTemperatureUnitChanged: {
                if (settingsViewDialog.visible == true) {
                    weatherView.changeTempUnit(settingsView.temperatureUnit)
                }
            }
            onSpeedUnitChanged: {
                if (settingsViewDialog.visible == true) {
                    weatherView.changeSpeedUnit(settingsView.speedUnit)
                }
            }
            onApiChanged: {
                if (settingsViewDialog.visible == true) {
                    weatherView.weatherApi = api
                }
            }
            onShowCredits: creditsViewDialog.visible = true
            Keys.onEscapePressed: settingsViewDialog.visible = false
        }
        onVisibilityChanged: {
            if (visible == true)
                settingsView.forceActiveFocus()
        }
    }

    ApplicationWindow {
        id: creditsViewDialog
        width: settingsView.width
        height: settingsView.height
        visible: false
        flags: Qt.Dialog
        CreditsView {
            id: creditsView
            anchors.fill: parent
            visible: true
            textColor: applicationSettingsController.textColor
            backgroundColor: applicationSettingsController.applicationBackground
            Keys.onEscapePressed: creditsViewDialog.visible = false
        }
        onVisibilityChanged: {
            if (visible == true)
                creditsView.forceActiveFocus()
        }
    }

    TrayController {
        id: trayController
        trayVisibility: applicationSettingsController.trayVisibility
        trayTheme: applicationSettingsController.trayTheme
        icon: weatherView.tempValue
        onCloseApp: Qt.quit()
        onShowGui: mainWindow.visible = true
    }

    Timer {
        id: timer
        interval: 3600000
        running: true
        repeat: true
        onTriggered: weatherView.updateWeather()
    }
}
