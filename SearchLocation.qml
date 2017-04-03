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
import QtQuick.Window 2.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.4

import ownTypes.searchLocation 0.4

Item {
    id: root
    signal locationSelected()
    property string backgroundColor: util.backgroundColor()
    property string textColor: util.textColor()

    FontLoader {
        id: ubuntuCondensed
        source: "fonts/Ubuntu-C.ttf"
    }

    Rectangle {
        id: body
        anchors.fill: parent
        color: root.backgroundColor

        TextField {
            id: searchField
            placeholderText: "Search"
            maximumLength: 120
            font.pixelSize: 22
            font.family: ubuntuCondensed.name
            width: parent.width * 90 / 100
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: (parent.width - width) / 2
            style: TextFieldStyle {
                textColor: root.textColor
                placeholderTextColor: root.textColor
                background: Rectangle {
                    border.width: 0
                    color: "transparent"
                }
            }

            onTextChanged: {
                locationSearcher.locationName = text
            }
        }

        Rectangle {
            id: searchFieldBottomBorder
            anchors.top: searchField.bottom
            anchors.topMargin: 0
            anchors.left: parent.left
            width: searchField.width
            height: 1
            anchors.leftMargin: (parent.width - width) / 2
            color: "#AAffffff"

        }

        ListView {
            id: searchList
            anchors.top: searchFieldBottomBorder.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: (parent.width - width) / 2
            width: searchField.width
            height: (body.height - searchField.height - 30)
            cacheBuffer: 0
            displayMarginBeginning: 0
            displayMarginEnd: 0
            clip: true
            delegate: Rectangle {
                height: 40
                width: searchList.width
                color: "transparent"
                Text {
                    id: delegateText
                    text: modelData
                    color: root.textColor
                    font.pixelSize: 18
                    anchors.fill: parent
                    verticalAlignment: Qt.AlignVCenter
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        locationSearcher.setLocation(delegateText.text);
                    }
                }
            }
        }
    }

    LocationSearchController {
        id: locationSearcher

        onLocationResultChanged: {
            searchList.model = locationSearcher.locationResult
        }

        onErrorChanged: {
            errorDialog.text = locationSearcher.error
            errorDialog.visible = true
        }

        onLocationChanged: {
            root.locationSelected();
        }
    }

    MessageDialog {
        id: errorDialog
        title: "Error"
        icon: StandardIcon.Critical
        visible: false
        standardButtons: StandardButton.Ok
    }
}
