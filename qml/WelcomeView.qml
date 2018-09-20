import QtQuick 2.0
import QtQuick.Controls 2.2

import "uMatControls" as CoreControls

Rectangle {
    id: window
    color: "#ff0099ff"
    signal finished()
    SwipeView {
        id: welcomeView
        anchors.fill: parent
        currentIndex: 0

        Image {
            id: welcomeScreen
            source: applicationPath + "other/screens/toggle-screens.png"
            width: window.width
            height: (window.height - 100)
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: forecastScreen
            source: applicationPath + "other/screens/main-screen-forecast.png"
            width: window.width
            height: (window.height - 100)
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: locationsScreen
            source: applicationPath + "other/screens/locations-screen.png"
            width: window.width
            height: (window.height - 100)
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            fillMode: Image.PreserveAspectFit
        }
        Image {
            id: locationsSearchScreen
            source: applicationPath + "other/screens/location-search.png"
            width: window.width
            height: (window.height - 100)
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            fillMode: Image.PreserveAspectFit
        }
    }

    PageIndicator {
        id: pageIndicator
        count: welcomeView.count
        currentIndex: welcomeView.currentIndex
        anchors.bottom: nextScreenButton.top
        anchors.bottomMargin: 10
        anchors.horizontalCenter: welcomeView.horizontalCenter
        interactive: true
        onCurrentIndexChanged: welcomeView.currentIndex = pageIndicator.currentIndex
    }

    CoreControls.Button {
        id: nextScreenButton
        anchors.right: welcomeView.right
        anchors.rightMargin: ((parent.width / 2) - (nextScreenButton.width / 2))
        anchors.bottom: welcomeView.bottom
        anchors.bottomMargin: 20
        width: (window.width * 0.7)
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
}
