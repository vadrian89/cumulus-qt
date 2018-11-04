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
import QtQuick.Layouts 1.0

Item {
    id: rootItem
    visible: false
    property string appVersion: "4.1.1"
    property string textColor
    property string backgroundColor
    property int contentWidth: rootItem.width * 90 / 100

    Rectangle {
        id: root
        color: rootItem.backgroundColor.length > 7 ? "#" + rootItem.backgroundColor.substring(3) : rootItem.backgroundColor
        width: rootItem.width
        height: rootItem.height
        Flickable {
            id: rootView
            anchors.fill: parent
            width: rootItem.width
            height: rootItem.height
            contentWidth: rootItem.width
            contentHeight: contentItem.height
            Rectangle {
                id: contentItem
                color: "transparent"
                width: rootItem.width
                height: childrenRect.height
                LinkedText {
                    id: versionText
                    height: 60
                    width: rootItem.contentWidth
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                    font.pixelSize: 26
                    color: rootItem.textColor
                    text: "Cumulus v" + appVersion
                    url: "https://vadrian89.github.io/cumulus-qt/"
                }
                LinkedText {
                    id: fontCreditsText
                    anchors.top: versionText.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: (rootItem.width - fontCreditsText.width) / 2
                    width: rootItem.contentWidth
                    font.pixelSize: 14
                    color: rootItem.textColor
                    wrapMode: Text.Wrap
                    text: qsTr("Thanks to Erik Flowers for his weather icons fonts!")
                    url: "https://erikflowers.github.io/weather-icons/"
                }
                LinkedText {
                    id: fontAwesomeCreditsText
                    anchors.top: fontCreditsText.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: (rootItem.width - fontAwesomeCreditsText.width) / 2
                    width: rootItem.contentWidth
                    font.pixelSize: 14
                    color: rootItem.textColor
                    wrapMode: Text.Wrap
                    text: qsTr("Thanks to Dave Gandy for Font Awesome!")
                    url: "http://fontawesome.io/"
                }
                Text {
                    id: creditsText
                    anchors.top: fontAwesomeCreditsText.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: (rootItem.width - creditsText.width) / 2
                    width: rootItem.contentWidth
                    font.pixelSize: 14
                    color: rootItem.textColor
                    wrapMode: Text.Wrap
                    text: qsTr("Cumulus is a simple weather application based on Qt framework.<br> ")
                          + qsTr("It is and always be free.<br>")
                          + qsTr("Source code released under GPL-3.<br>")
                          + qsTr("Forked from Typhoon, which was based of Stormcloud.<br>")
                          + qsTr("Active developers: <ul><li>Adrian Verban verbanady@gmail.com</li></ul>")
                          + qsTr("Special thanks to: <ul><li>Daryl Bennett</li><li>Archisman Panigrahi</li></ul>")
                          + qsTr("Weather data provided by: ")
                          + "<ul><li>Open Weather Map</li>"
                          + "<li>Yahoo! Weather</li>"
                          + "<li>Weather Underground</li>"
                          + "</ul>"
                          + qsTr("Icons credits:")
                }
                ListView {
                    id: iconCreditsList
                    anchors.top: creditsText.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: (rootItem.width - iconCreditsList.width) / 2
                    height: 300
                    width: rootItem.contentWidth
                    orientation: ListView.Vertical
                    clip: true
                    spacing: 0
                    snapMode: ListView.SnapToItem
                    model: util.creditsList()
                    visible: true
                    delegate: CreditsListDelegate {
                        height: 60
                        width: iconCreditsList.width
                        textColor: rootItem.textColor
                        icon: model.modelData.icon
                        author: model.modelData.author
                        authorUrl: model.modelData.authorUrl
                        license: model.modelData.license
                    }
                }
            }
        }
    }
}
