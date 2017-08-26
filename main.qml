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
import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Window 2.2

import ownTypes.settingsController 1.0
import ownTypes.TrayController 0.3

ApplicationWindow {
    id: mainWindow
    property int winMinHeight: 150
    property int winMinWidth: 140
    property int widthBreakPoint: 170
    minimumHeight: winMinHeight
    minimumWidth: winMinWidth
    height: applicationSettingsController.windowHeight
    width: applicationSettingsController.windowWidth
    title: qsTr("Cumulus")
    color: "transparent"
    visible: util.trayVisibility() == true ? false : true
    x: applicationSettingsController.windowX
    y: applicationSettingsController.windowY
    flags: Qt.FramelessWindowHint
    background: Rectangle {
        color: applicationSettingsController.applicationBackground
    }

    onHeightChanged: applicationSettingsController.windowHeight = mainWindow.height
    onWidthChanged: applicationSettingsController.windowWidth = mainWindow.width
    onClosing: {
        if (applicationSettingsController.trayVisibility == true) {
            visible = false
        }
        else {
            Qt.quit()
        }
    }

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
        moveControlAlias.acceptedButtons: Qt.LeftButton
        property point clickPos
        onCloseButtonClicked: mainWindow.close()
        onMinimizeButtonClicked: mainWindow.visibility = Window.Minimized
        onRefreshButtonClicked: weatherViewLoader.item.updateWeather()
        onMenuButtonClicked: settingsViewDialog.visible = true
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
            moveControlAlias.cursorShape = Qt.ArrowCursor
            mainWindow.maximumHeight = Screen.desktopAvailableHeight
            mainWindow.maximumWidth = Screen.desktopAvailableWidth
            mainWindow.minimumHeight = winMinHeight
            mainWindow.minimumWidth = winMinWidth
            applicationSettingsController.windowX = mainWindow.x
            applicationSettingsController.windowY = mainWindow.y
        }
    }
    Component.onCompleted: weatherViewLoader.source = "WeatherWindow.qml"

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
                onNetworkError: {
                    timer.interval = 60000
                }
            }
        }
        Component.onCompleted: weatherViewLoader.item.updateWeather()
    }


    TrayController {
        id: trayController
        trayVisibility: applicationSettingsController.trayVisibility
        trayTheme: applicationSettingsController.trayTheme
        icon: weatherViewLoader.item.tempValue
        onCloseApp: Qt.quit()
        onShowGui: mainWindow.visible = true
    }

    Timer {
        id: timer
        interval: 3600000
        running: true
        repeat: true
        onTriggered: weatherViewLoader.item.updateWeather()
    }

    ApplicationWindow {
        id: settingsViewDialog
        width: 300
        height: 500
        visible: false
        modality: Qt.WindowModal
        SettingsWindow {
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

    ApplicationWindow {
        id: creditsViewDialog
        width: settingsView.width
        height: settingsView.height
        visible: false
        modality: Qt.WindowModal
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

    ResizeController {
        id: bottomRController
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 20
        height: width
        cursorShape: Qt.SizeFDiagCursor
        onXResizeChanged: {
            if (xResize > 0) {
                mainWindow.width += xResize
            }
            else {
                if (mainWindow.width > mainWindow.minimumWidth) {
                    mainWindow.width += xResize
                }
            }
        }

        onYResizeChanged: {
            if (yResize > 0) {
                mainWindow.height += yResize
            }
            else {
                if (mainWindow.height > mainWindow.minimumHeight) {
                    mainWindow.height += yResize
                }
            }
        }
    }

    ResizeController {
        id: bottomController
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: 20
        width: parent.width - 40
        height: 5
        cursorShape: Qt.SizeVerCursor
        onYResizeChanged: {
            if (yResize > 0) {
                mainWindow.height += yResize
            }
            else {
                if (mainWindow.height > mainWindow.minimumHeight) {
                    mainWindow.height += yResize
                }
            }
        }
    }

    ResizeController {
        id: rightController
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.right: parent.right
        width: 5
        height: parent.height - 20
        cursorShape: Qt.SizeHorCursor
        onXResizeChanged: {
            if (xResize > 0) {
                mainWindow.width += xResize
            }
            else {
                if (mainWindow.width > mainWindow.minimumWidth) {
                    mainWindow.width += xResize
                }
            }
        }
    }

    ResizeController {
        id: bottomLController
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 20
        height: width
        cursorShape: Qt.SizeBDiagCursor
        onXResizeChanged: {
            if (xResize < 0) {
                mainWindow.width -= xResize
                mainWindow.x += xResize
            }
            else {
                if (mainWindow.width > mainWindow.minimumWidth) {
                    mainWindow.width -= xResize
                    mainWindow.x += xResize
                }
            }
        }
        onYResizeChanged: {
            if (yResize > 0) {
                mainWindow.height += yResize
            }
            else {
                if (mainWindow.height > mainWindow.minimumHeight) {
                    mainWindow.height += yResize
                }
            }
        }
    }

    ResizeController {
        id: leftController
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.left: parent.left
        width: 5
        height: parent.height - 20
        cursorShape: Qt.SizeHorCursor
        onXResizeChanged: {
            if (xResize < 0) {
                mainWindow.width -= xResize
                mainWindow.x += xResize
            }
            else {
                if (mainWindow.width > mainWindow.minimumWidth) {
                    mainWindow.width -= xResize
                    mainWindow.x += xResize
                }
            }
        }
    }
}
