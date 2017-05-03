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

Item {
    id: rootDialog
    visible: false
    property string appVersion: "3.0.2"
    property string textColor: util.textColor()
    property string backgroundColor: util.backgroundColor()
    property int contentWidth: rootDialog.width * 90 / 100

    Rectangle {
        id: root
        color: backgroundColor
        width: rootDialog.width
        height: rootDialog.height
        Flickable {
            id: rootView
            anchors.fill: parent
            width: rootDialog.width
            height: rootDialog.height
            contentWidth: rootDialog.width
            contentHeight: contentItem.height
            Rectangle {
                id: contentItem
                color: "transparent"
                width: rootDialog.width
                height: childrenRect.height
                LinkedText {
                    id: versionText
                    height: 60
                    width: rootDialog.contentWidth
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                    font.pixelSize: 26
                    color: rootDialog.textColor
                    text: "Cumulus v" + appVersion
                    url: "https://vadrian89.github.io/cumulus-qt/"
                }
                LinkedText {
                    id: fontCreditsText
                    anchors.top: versionText.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: (rootDialog.width - fontCreditsText.width) / 2
                    width: rootDialog.contentWidth
                    font.pixelSize: 14
                    color: rootDialog.textColor
                    wrapMode: Text.Wrap
                    text: "The Weather Icons created and maintained by Erik Flowers"
                    url: "https://erikflowers.github.io/weather-icons/"
                }
                Text {
                    id: creditsText
                    anchors.top: fontCreditsText.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: (rootDialog.width - creditsText.width) / 2
                    width: rootDialog.contentWidth
                    font.pixelSize: 14
                    color: rootDialog.textColor
                    wrapMode: Text.Wrap
                    text: "Cumulus is a simple weather application based on Qt framework.<br> "
                          + "It is and always be free.<br>"
                          + "Source code released under GPL-3.<br>"
                          + "Forked from Typhoon, which was based of Stormcloud.<br>"
                          + "Active developers: <ul><li>Adrian Verban verbanady@gmail.com</li></ul>"
                          + "Special thanks to: <ul><li>Daryl Bennett</li><li>Archisman Panigrahi</li></ul>"
                          + "Weather provided by: <ul><li>Open Weather Map</li><li>Yahoo! Weather</li></ul>"
                          + "Icons credits:"
                }
                ListView {
                    id: iconCreditsList
                    anchors.top: creditsText.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: (rootDialog.width - iconCreditsList.width) / 2
                    height: 300
                    width: rootDialog.contentWidth
                    orientation: ListView.Vertical
                    clip: true
                    spacing: 0
                    snapMode: ListView.SnapToItem
                    model: util.creditsList()
                    visible: true
                    delegate: CreditsListDelegate {
                        height: 60
                        width: iconCreditsList.width
                        textColor: rootDialog.textColor
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
