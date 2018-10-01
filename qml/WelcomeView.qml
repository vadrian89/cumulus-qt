import QtQuick 2.0
import QtQuick.Controls 2.2

import "uMatControls" as CoreControls

Rectangle {
    id: window
    color: "#ff0099ff"
    signal finished()
    SwipeView {
        id: welcomeView
        anchors.top: window.top
        anchors.left: window.left
        width: window.width
        height: (window.height - nextScreenButton.height - nextScreenButton.anchors.topMargin - pageIndicator.height - pageIndicator.anchors.topMargin)
        currentIndex: 0

        Image {
            id: welcomeScreen
            source: util.assetsPath() + "screens/toggle-screens.png"
            width: window.width
            height: (window.height - 100)
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: forecastScreen
            source: util.assetsPath() + "screens/main-screen-forecast.png"
            width: window.width
            height: (window.height - 100)
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: locationsScreen
            source: util.assetsPath() + "screens/locations-screen.png"
            width: window.width
            height: (window.height - 100)
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            fillMode: Image.PreserveAspectFit
        }
        Image {
            id: locationsSearchScreen
            source: util.assetsPath() + "screens/location-search.png"
            width: window.width
            height: (window.height - 100)
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            fillMode: Image.PreserveAspectFit
        }
    }

    CoreControls.Button {
        id: nextScreenButton
        anchors.top: welcomeView.bottom
        anchors.topMargin: 20
        anchors.left: window.left
        anchors.leftMargin: ((parent.width / 2) - (nextScreenButton.width / 2))
        width: (window.width * 0.7)
        height: 50
        backgroundColor: "#3d8fb5"
        text.text: (welcomeView.currentIndex < 3 ? ("Next ") : ("Done ")) + (" >")
        text.color: "#ffffff"
        text.wrapMode: Text.Wrap
        dropShadow.verticalOffset: 3
        ripple: true
        onClicked: {
            if (welcomeView.currentIndex < 3) {
                welcomeView.currentIndex += 1
            }
            else {
                welcomeView.currentIndex = 0
                window.finished()
            }
        }
    }

    PageIndicator {
        id: pageIndicator
        count: welcomeView.count
        currentIndex: welcomeView.currentIndex
        anchors.top: nextScreenButton.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: window.horizontalCenter
        interactive: true
        onCurrentIndexChanged: welcomeView.currentIndex = pageIndicator.currentIndex
    }
}
