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

import ownTypes.settingsController 0.3

ApplicationWindow {
    id: mainWindow
    visible: true
    title: qsTr("SWeather")
    color: "transparent"

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

        ApplicationBar {
            id: applicationBar
            anchors.top: appBody.top
            width: appBody.width
            height: parent.height * 10 / 100
            textColor: applicationSettingsController.textColor
            iconsFont: weatherIcons.name
            onMenuButtonClicked: {
                if(bodyView.contentX != 0) {
                    bodyView.contentX = 0
                }
                else {
                    bodyView.contentX = bodyView.width
                }
            }
            onCloseButtonClicked: Qt.quit()
            onRefreshButtonClicked: weatherView.updateWeather()
        }

        Flickable {
            id: bodyView
            anchors.top: applicationBar.bottom
            width: appBody.width
            height: appBody.height - applicationBar.height
            contentHeight: height
            contentWidth: width * 2
            interactive: false
            contentX: bodyView.width

            SettingsWindow {
                id: settingsView
                width: bodyView.width
                height: bodyView.height
                onLocationChanged: {
                    weatherView.updateWeather()
                    bodyView.contentX = settingsView.width
                }
                onWidthChanged: {
                    if(bodyView.contentX != 0) {
                        bodyView.contentX = settingsView.width
                    }
                }
            }

            WeatherWindow {
                id: weatherView
                width: bodyView.width
                height: bodyView.height
                anchors.left: settingsView.right
                textColor: applicationSettingsController.textColor
                textFontFamily: ubuntuCondensed.name
                iconsFont: weatherIcons.name
                Keys.onBackPressed: Qt.quit()
            }
        }
    }
}
