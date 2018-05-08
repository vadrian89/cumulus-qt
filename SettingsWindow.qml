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
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

Item {
    id: root
    visible: false
    property alias settingsFlickAlias: settingsFlick
    property bool trayVisible: root.trayVisible ? root.trayVisible : false
    property alias trayTheme: trayColorSwitch.state
    property alias windowControls: winControlSwitch.state
    property alias loginStart: startUpSwitch.checked
    property string backgroundColor: backgroundColor ? backgroundColor : "#ffffff"
    property string textColor: textColor ? textColor : "#ffffff"
    property string speedUnit
    property string temperatureUnit
    property string api
    property int settingsItemHeight: 64
    property int textFontSize: 14
    property string iconsFont: "Arial"
    property int switchHeight: 25
    property int switchWidth: 70
    property bool useGps: false
    signal locationChanged()
    signal showCredits()

    Rectangle {
        id: settingsOptionsView
        anchors.fill: parent
        color: root.backgroundColor.length > 7 ? ("#" + root.backgroundColor.substring(3)) : root.backgroundColor

        Flickable {
            id: settingsFlick
            contentHeight: settingsBody.childrenRect.height
            contentWidth: root.width
            width: root.width
            height: root.height
            Rectangle {
                id: settingsBody
                width: settingsFlick.width
                height: settingsFlick.height
                color: "transparent"

                SettingsOptionItem {
                    id: tempUnitItem
                    optionText: "Temperature Unit"
                    backgroundColor: "#ffffff"
                    anchors.top: settingsBody.top
                    height: root.settingsItemHeight
                    width: settingsBody.width
                    iconUrl: util.iconPathPrefix() + "temperature_icon.png"
                    onClicked: {
                        if (tempUnitSelect.visible == false) {
                            tempUnitSelect.currentIndex = tempUnitSelect.find(util.tempUnitSymbol(root.temperatureUnit).trim())
                            tempUnitSelect.visible = true
                        }
                        else {
                            tempUnitSelect.visible = false
                        }
                    }
                }

                ComboBox {
                    id: tempUnitSelect
                    anchors.top: tempUnitItem.bottom
                    anchors.topMargin: 5
                    width: tempUnitItem.width / 2
                    anchors.left: parent.left
                    anchors.leftMargin: width / 2
                    visible: false
                    model: [ "°F", "°C" ]
                    delegate: ItemDelegate {
                        width: tempUnitSelect.width
                        height: tempUnitSelect.height
                        contentItem: Text {
                            text: modelData
                            font.pixelSize: root.textFontSize
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                    }
                    onVisibleChanged: {
                        if(tempUnitSelect.visible == false) {
                            speedUnitItem.anchors.top = tempUnitItem.bottom
                            speedUnitItem.anchors.topMargin = 0
                            tempUnitItem.bottomBorderVisibility = true
                        }
                        else {
                            speedUnitItem.anchors.top = tempUnitSelect.bottom
                            speedUnitItem.anchors.topMargin = 5
                            tempUnitItem.bottomBorderVisibility = false
                        }
                    }
                    onCurrentIndexChanged: {
                        if (visible == true) {
                            switch (currentIndex) {
                            case 0:
                                root.temperatureUnit = "f"
                                break;
                            case 1:
                                root.temperatureUnit = "c"
                                break;
                            default:
                                root.temperatureUnit = "k"
                                break;
                            }
                        }
                    }
                }

                SettingsOptionItem {
                    id: speedUnitItem
                    optionText: "Speed Unit"
                    backgroundColor: "#ffffff"
                    anchors.top: tempUnitItem.bottom
                    height: root.settingsItemHeight
                    width: settingsBody.width
                    iconUrl: util.iconPathPrefix() + "speed_icon.png"
                    onClicked: {
                        if (speedUnitSelect.visible == false) {
                            speedUnitSelect.currentIndex = speedUnitSelect.find(util.speedUnitSymbol(root.speedUnit).trim())
                            speedUnitSelect.visible = true
                        }
                        else {
                            speedUnitSelect.visible = false
                        }
                    }
                }

                ComboBox {
                    id: speedUnitSelect
                    anchors.top: speedUnitItem.bottom
                    anchors.topMargin: 5
                    width: tempUnitItem.width / 2
                    anchors.left: parent.left
                    anchors.leftMargin: width / 2
                    visible: false
                    model: [ "MPH", "Km/h", "m/s" ]
                    onVisibleChanged: {
                        if(speedUnitSelect.visible == false) {
                            colorPalettesItem.anchors.top = speedUnitItem.bottom
                            colorPalettesItem.anchors.topMargin = 0
                            speedUnitItem.bottomBorderVisibility = true
                        }
                        else {
                            colorPalettesItem.anchors.top = speedUnitSelect.bottom
                            colorPalettesItem.anchors.topMargin = 5
                            speedUnitItem.bottomBorderVisibility = false
                        }
                    }
                    onCurrentIndexChanged: {
                        if (visible == true) {
                            switch (currentIndex) {
                            case 0:
                                root.speedUnit = "mph"
                                break;
                            case 1:
                                root.speedUnit = "kph"
                                break;
                            default:
                                root.speedUnit = "m/s"
                                break;
                            }
                        }
                    }
                }

                SettingsOptionItem {
                    id: colorPalettesItem
                    optionText: "Colors"
                    backgroundColor: "#ffffff"
                    anchors.top: speedUnitItem.bottom
                    height: root.settingsItemHeight
                    width: settingsBody.width
                    iconUrl: util.iconPathPrefix() + "palette.png"
                    onClicked: {
                        if (colorButtonsLayout.visible == false) {
                            colorButtonsLayout.visible = true
                        }
                        else {
                            colorButtonsLayout.visible = false
                        }
                    }
                    bottomBorderVisibility: true
                }

                GridLayout {
                    id: colorButtonsLayout
                    columns: 2
                    anchors.top: colorPalettesItem.bottom
                    width: settingsBody.width * 80 / 100
                    anchors.left: parent.left
                    anchors.leftMargin: (settingsBody.width - width) / 2
                    visible: false
                    ColorPalleteButton {
                        id: bgColorButton
                        Layout.fillHeight: false
                        Layout.fillWidth: true
                        Layout.preferredHeight: 70
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                        text: "Background"
                        textColor: root.textColor
                        backgroundColor: root.backgroundColor
                        onClicked: {
                            colorDialog.purpose = "background"
                            colorDialog.showAlphaChannel = true
                            colorDialog.color = root.backgroundColor
                            colorDialog.visible = true
                        }
                    }
                    ColorPalleteButton {
                        id: txtColorButton
                        Layout.fillHeight: false
                        Layout.fillWidth: true
                        Layout.preferredHeight: 70
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                        text: "Text"
                        textColor: root.textColor
                        backgroundColor: root.textColor
                        onClicked: {
                            colorDialog.purpose = "text"
                            colorDialog.color = root.textColor
                            colorDialog.visible = true
                        }
                    }
                    onVisibleChanged: {
                        if(colorButtonsLayout.visible == false) {
                            apiSelectItem.anchors.top = colorPalettesItem.bottom
                            apiSelectItem.anchors.topMargin = 0
                            colorPalettesItem.bottomBorderVisibility = true
                        }
                        else {
                            apiSelectItem.anchors.top = colorButtonsLayout.bottom
                            apiSelectItem.anchors.topMargin = 5
                            colorPalettesItem.bottomBorderVisibility = false
                        }
                    }
                }

                SettingsOptionItem {
                    id: apiSelectItem
                    optionText: "Provider"
                    backgroundColor: "#ffffff"
                    anchors.top: colorPalettesItem.bottom
                    height: root.settingsItemHeight
                    width: settingsBody.width
                    iconUrl: util.iconPathPrefix() + "api.png"
                    onClicked: {
                        if (apiSelect.visible == false) {
                            apiSelect.visible = true
                        }
                        else {
                            apiSelect.visible = false
                        }
                    }
                }

                ComboBox {
                    id: apiSelect
                    anchors.top: apiSelectItem.bottom
                    anchors.topMargin: 5
                    width: Math.round(apiSelectItem.width * 60 / 100)
                    anchors.left: parent.left
                    anchors.leftMargin: (parent.width - apiSelect.width) / 2
                    visible: false
                    model: ["Open Weather Map", "Yahoo! Weather", "Weather Underground"]
                    onCurrentIndexChanged: {
                        if (visible == true) {
                            switch (currentIndex) {
                            case 1:
                                root.api = "y"
                                break;
                            case 2:
                                root.api = "wund"
                                break;
                            default:
                                root.api = "owm"
                                break;
                            }
                        }
                    }
                    onVisibleChanged: {
                        if(apiSelect.visible == false) {
                            creditsItem.anchors.top = apiSelectItem.bottom
                            creditsItem.anchors.topMargin = 0
                            apiSelectItem.bottomBorderVisibility = true
                        }
                        else {
                            creditsItem.anchors.top = apiSelect.bottom
                            creditsItem.anchors.topMargin = 5
                            apiSelectItem.bottomBorderVisibility = false
                        }
                    }
                }
                Component.onCompleted: {
                    if (settingsController.getWeatherApi() == "y") {
                        apiSelect.currentIndex = 1
                    }
                    else if (settingsController.getWeatherApi() == "wund") {
                        apiSelect.currentIndex = 2
                    }
                    else {
                        apiSelect.currentIndex = 0
                    }
                }

                SettingsOptionItem {
                    id: creditsItem
                    optionText: "Credits"
                    backgroundColor: "#ffffff"
                    anchors.top: apiSelectItem.bottom
                    height: root.settingsItemHeight
                    width: settingsBody.width
                    iconUrl: util.iconPathPrefix() + "credits.png"
                    bottomBorderVisibility: false
                    onClicked: root.showCredits()
                }

                CustomSwitch {
                    id: gpsSwitch
                    width: settingsBody.width
                    height: 50
                    anchors.top: creditsItem.bottom
                    anchors.left: parent.left
                    labelColor: root.textColor
                    switchRailWidth: root.switchWidth
                    switchRailHeight: root.switchHeight
                    switchLabel: qsTr("Use GPS")
                    state: root.useGps == true ? "right" : "left"
                    visible: true
                    onCheckedChanged: root.useGps = checked
                }

                CustomSwitch {
                    id: traySwitch
                    width: settingsBody.width
                    height: 50
                    anchors.top: gpsSwitch.bottom
                    anchors.left: parent.left
                    labelColor: root.textColor
                    switchRailWidth: root.switchWidth
                    switchRailHeight: root.switchHeight
                    switchLabel: qsTr("Close To Tray")
                    state: root.trayVisible == true ? "right" : "left"
                    visible: util.osType() === "android" ? false : true
                    onCheckedChanged: root.trayVisible = checked
                }

                CustomSwitch {
                    id: trayColorSwitch
                    width: settingsBody.width
                    height: 50
                    anchors.top: traySwitch.bottom
                    anchors.left: parent.left
                    labelColor: root.textColor
                    switchRailWidth: root.switchWidth
                    switchRailHeight: root.switchHeight
                    switchLabel: qsTr("Tray theme")
                    visible: util.osType() === "android" ? false : true
                    leftText: "\uf002"
                    leftTextColor: "black"
                    leftStateValue: "light"
                    rightText: "\uf002"
                    rightTextColor: "white"
                    rightStateValue: "dark"
                    checkedColor: "gray"
                    uncheckedColor: "gray"
                    font: root.iconsFont
                }

                CustomSwitch {
                    id: winControlSwitch
                    width: settingsBody.width
                    height: 50
                    anchors.top: trayColorSwitch.bottom
                    anchors.left: parent.left
                    labelColor: root.textColor
                    switchRailWidth: root.switchWidth
                    switchRailHeight: root.switchHeight
                    switchLabel: qsTr("Window controls position")
                    visible: util.osType() === "android" ? false : true
                    leftText: "\uf04d"
                    rightText: "\uf048"
                    checkedColor: "gray"
                    uncheckedColor: "gray"
                    font: root.iconsFont
                }

                CustomSwitch {
                    id: startUpSwitch
                    width: settingsBody.width
                    height: 50
                    anchors.top: winControlSwitch.bottom
                    anchors.left: parent.left
                    labelColor: root.textColor
                    switchRailWidth: root.switchWidth
                    switchRailHeight: root.switchHeight
                    switchLabel: qsTr("Start on login")
                    state: settingsController.loginStartCheck() == true ? "right" : "left"
                    visible: util.osType() === "android" ? false : true
                }
            }
        }
    }

    ColorDialog {
        id: colorDialog
        title: "Please choose a color"
        showAlphaChannel: purpose == "background" ? true : false
        modality: Qt.WindowModal
        property string purpose
        onVisibleChanged: {
            if(visible == false) {
                root.focus = true
                root.forceActiveFocus()
            }
            else {
                root.focus = false
            }
        }
        onAccepted: {
            if (purpose == "background") {
                root.backgroundColor = currentColor
            }
            else {
                root.textColor = currentColor
            }
        }
    }
}

