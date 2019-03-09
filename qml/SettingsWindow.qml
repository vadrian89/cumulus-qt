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
import QtQuick.Controls 1.4
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

import "CustomControls" as CoreControls

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
    property string speedUnit: ""
    property string temperatureUnit: ""
    property string api: ""
    property string apiKey: "Insert key"
    property int settingsItemHeight: 64
    property int textFontSize: 14
    property string iconsFont: "Arial"
    property int switchHeight: 25
    property int switchWidth: 70
    property bool useGps: false
    signal locationChanged()
    signal showCredits()
    signal closeSettingsView()

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

                CoreControls.SettingsItem {
                    id: tempUnitItem
                    text.text: qsTr("Temperature Unit")
                    text.font.pixelSize: root.textFontSize
                    backgroundColor: "#ffffff"
                    anchors.top: settingsBody.top
                    height: root.settingsItemHeight
                    width: settingsBody.width
                    icon.source: util.iconPathPrefix() + "temperature_icon.png"
                    icon.visible: true
                    ripple: true
                    summaryText.text: util.tempUnitSymbol(root.temperatureUnit).trim()
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
                    onVisibleChanged: {
                        if(tempUnitSelect.visible == false) {
                            speedUnitItem.anchors.top = tempUnitItem.bottom
                            speedUnitItem.anchors.topMargin = 0
                        }
                        else {
                            speedUnitItem.anchors.top = tempUnitSelect.bottom
                            speedUnitItem.anchors.topMargin = 5
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

                CoreControls.SettingsItem {
                    id: speedUnitItem
                    text.text: qsTr("Speed Unit")
                    text.font.pixelSize: root.textFontSize
                    backgroundColor: "#ffffff"
                    anchors.top: tempUnitItem.bottom
                    height: root.settingsItemHeight
                    width: settingsBody.width
                    icon.source: util.iconPathPrefix() + "speed_icon.png"
                    icon.visible: true
                    ripple: true
                    summaryText.text: util.speedUnitSymbol(root.speedUnit).trim()
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
                        }
                        else {
                            colorPalettesItem.anchors.top = speedUnitSelect.bottom
                            colorPalettesItem.anchors.topMargin = 5
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

                CoreControls.SettingsItem {
                    id: colorPalettesItem
                    text.text: qsTr("Colors")
                    text.font.pixelSize: root.textFontSize
                    backgroundColor: "#ffffff"
                    anchors.top: speedUnitItem.bottom
                    height: root.settingsItemHeight
                    width: settingsBody.width
                    icon.source: util.iconPathPrefix() + "palette.png"
                    icon.visible: true
                    ripple: true
                    summaryText.visible: false
                    onClicked: {
                        if (colorButtonsLayout.visible == false) {
                            colorButtonsLayout.visible = true
                        }
                        else {
                            colorButtonsLayout.visible = false
                        }
                    }
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
                        text: qsTr("Background")
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
                        text: qsTr("Text")
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
                        }
                        else {
                            apiSelectItem.anchors.top = colorButtonsLayout.bottom
                            apiSelectItem.anchors.topMargin = 5
                        }
                    }
                }

                CoreControls.SettingsItem {
                    id: apiSelectItem
                    text.text: qsTr("Provider")
                    text.font.pixelSize: root.textFontSize
                    backgroundColor: "#ffffff"
                    anchors.top: colorPalettesItem.bottom
                    height: root.settingsItemHeight
                    width: settingsBody.width
                    icon.source: util.iconPathPrefix() + "api.png"
                    icon.visible: true
                    summaryText.text: "Open Weather Map"
                    ripple: true
                }

                CoreControls.SettingsItem {
                    id: apiKeyItem
                    text.text: qsTr("Provider key")
                    text.font.pixelSize: root.textFontSize
                    backgroundColor: "#ffffff"
                    anchors.top: apiSelectItem.bottom
                    height: root.settingsItemHeight
                    width: settingsBody.width
                    icon.source: util.iconPathPrefix() + "key.png"
                    icon.visible: true
                    summaryText.text: root.apiKey
                    ripple: true
                    onClicked: apiKeyDialog.visible = true
                }

//                ComboBox {
//                    id: apiSelect
//                    anchors.top: apiSelectItem.bottom
//                    anchors.topMargin: 5
//                    width: Math.round(apiSelectItem.width * 60 / 100)
//                    anchors.left: parent.left
//                    anchors.leftMargin: (parent.width - apiSelect.width) / 2
//                    visible: false
//                    model: ["Open Weather Map", "Yahoo! Weather", "Weather Underground"]
//                    onCurrentIndexChanged: {
//                        if (visible == true) {
//                            if (currentIndex == 1)
//                                root.api = "wund"
//                            else
//                                root.api = "owm"
//                        }
//                    }
//                    onVisibleChanged: {
//                        if(apiSelect.visible == false) {
//                            creditsItem.anchors.top = apiSelectItem.bottom
//                            creditsItem.anchors.topMargin = 0
//                        }
//                        else {
//                            creditsItem.anchors.top = apiSelect.bottom
//                            creditsItem.anchors.topMargin = 5
//                        }
//                    }
//                }
//                Component.onCompleted: (settingsController.getWeatherApi() === "wund")  ? apiSelect.currentIndex = 1 : apiSelect.currentIndex = 0

                CoreControls.SettingsItem {
                    id: creditsItem
                    text.text: qsTr("Credits")
                    text.font.pixelSize: root.textFontSize
                    backgroundColor: "#ffffff"
                    anchors.top: apiKeyItem.bottom
                    height: root.settingsItemHeight
                    width: settingsBody.width
                    icon.source: util.iconPathPrefix() + "credits.png"
                    icon.visible: true
                    ripple: true
                    summaryText.visible: false
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
                    anchors.top: util.osType() === "android" ? gpsSwitch.bottom : trayColorSwitch.bottom
                    anchors.left: parent.left
                    labelColor: root.textColor
                    switchRailWidth: root.switchWidth
                    switchRailHeight: root.switchHeight
                    switchLabel: qsTr("Window controls position")
                    visible: true
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
                    state: settingsController.loginStartCheck() === true ? "right" : "left"
                    visible: util.osType() === "android" ? false : true
                }
            }
        }
    }

    Rectangle {
        id: settingsDialogWrapper
        anchors.fill: parent
        color: "#000000"
        opacity: 0.3
        visible: false
        MouseArea {
            anchors.fill: parent
        }
    }

    Rectangle {
        id: apiKeyDialog
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: (parent.height / 2) - (apiKeyDialog.height / 2)
        width: parent.width
        height: 100
        visible: false
        Text {
            id: apiKeyDialogTitle
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            width: parent.width
            height: 30
            font.pixelSize: 16
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            text: qsTr("Enter API key")
            color: root.backgroundColor
        }
        TextInput {
            id: apiKeyDialogInput
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: apiKeyDialogTitle.bottom
            width: parent.width - 20
            font.pixelSize: 16
            text: root.apiKey
            autoScroll: false
            clip: true
            color: apiKeyDialogOkButton.textColor
            selectedTextColor: root.textColor
            selectionColor: root.backgroundColor
            selectByMouse: true
            Rectangle {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: apiKeyDialogOkButton.textColor
            }
            onAccepted: {
                if (text.trim().length == 0) {
                    messageDialog.title = qsTr("Error when saving API key!")
                    messageDialog.text = qsTr("Key cannot be empty!")
                    messageDialog.visible = true
                }
                else {
                    root.apiKey = apiKeyDialogInput.text
                    apiKeyDialog.visible = false
                }
            }
        }
        CoreControls.CustomButton {
            id: apiKeyDialogOkButton
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: apiKeyDialogInput.bottom
            anchors.topMargin: 10
            text.text: qsTr("Ok")
            text.font.pixelSize: 16
            textColor: root.backgroundColor
            backgroundColor: root.textColor
            width: 100
            height: 30
            dropShadow.visible: false
            ripple: true
            onClicked: apiKeyDialogInput.accepted()
        }
        CoreControls.CustomButton {
            id: apiKeyDialogCancelButton
            anchors.right: apiKeyDialogOkButton.left
            anchors.rightMargin: apiKeyDialogOkButton.anchors.rightMargin
            anchors.top: apiKeyDialogInput.bottom
            anchors.topMargin: apiKeyDialogOkButton.anchors.topMargin
            text.text: qsTr("Cancel")
            text.font.pixelSize: apiKeyDialogOkButton.text.font.pixelSize
            textColor: apiKeyDialogOkButton.textColor
            backgroundColor: apiKeyDialogOkButton.backgroundColor
            width: apiKeyDialogOkButton.width
            height: apiKeyDialogOkButton.height
            dropShadow.visible: false
            ripple: true
            onClicked: apiKeyDialog.visible = false
        }
        onVisibleChanged: {
            if(!visible) {
                settingsDialogWrapper.visible = false
                apiKeyDialogInput.focus = false
                root.focus = true
                root.forceActiveFocus()
            }
            else {
                settingsDialogWrapper.visible = true
                root.focus = false
                apiKeyDialogInput.focus = true
                apiKeyDialogInput.forceActiveFocus()
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
    MessageDialog {
        id: messageDialog
        title: "No message Dialog"
        text: "Nothing to see here."
        icon: StandardIcon.Warning
        standardButtons: StandardButton.Ok
    }
    Keys.onEscapePressed: {
        if (apiKeyDialogInput.focus)
            apiKeyDialog.visible = false
        else
            root.closeSettingsView()
    }
    Keys.onBackPressed: {
        if (apiKeyDialogInput.focus)
            apiKeyDialog.visible = false
        else
            root.closeSettingsView()
    }
}

