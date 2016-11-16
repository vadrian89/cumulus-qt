import QtQuick 2.6
import QtQuick.Window 2.2
import QtWebEngine 1.2
import QtQml 2.2

Window {
    id: window
    property point windowLocation: util.loadWindowLocation()
    property size windowSize: util.loadWindowSize()
    flags: Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint
    minimumHeight: windowSize.height
    minimumWidth: windowSize.width
    maximumHeight: windowSize.height
    maximumWidth: windowSize.width
    x: windowLocation.x
    y: windowLocation.y
    visible: false

    WebEngineView {
        property string systemLanguage: Qt.locale().name.substring(0,2)
        anchors.fill: parent
        url: "data/app.html?lang=" + systemLanguage

        Component.onCompleted: {
            window.visible = true
        }

        onWindowCloseRequested: window.close()
        onJavaScriptConsoleMessage: {
            console.log("Cumulus javascript log: " + message)
            if (message == "minimize") {
                window.visibility = Window.Minimized
            }
            else if(message.substring(0,3) == "url") {
                Qt.openUrlExternally(message.substring(3,message.length))
            }
        }
        onTitleChanged: {
            console.log("Cumulus title changed: " + title)
            if (title == "disabledrag") {
                appMouseArea.visible = false
            }
            else if (title == "enabledrag") {
                appMouseArea.visible = true
            }
            else if (title.substring(0,1) == "o") {
                window.opacity = Number(title.substring(1,5))
            }
            else if (title == "enable_launcher" || title == "disable_launcher") {
                util.toggleLauncherCount(title)
            }
            else if (title.substring(0,1) == "t") {
                util.setLauncherCount(title.substring(1,title.length))
            }
        }
    }

    MouseArea {
        id: appMouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        property point clickPos
        onPressed: {
            clickPos = Qt.point(mouse.x,mouse.y)
            cursorShape = Qt.ClosedHandCursor
        }
        onPositionChanged: {
            var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
            window.x = window.x + delta.x
            window.y = window.y + delta.y
        }
        onReleased: {
            if (window.x < 70 ) {
                window.x = 0
            }
            if (window.y < 70 ) {
                window.y = 0
            }
            util.saveWindowLocation(Qt.point(window.x, window.y));
            cursorShape = Qt.ArrowCursor
        }
    }
    onHeightChanged: {
        util.saveWindowSize(Qt.size(width, height))
    }

    onWidthChanged: {
        util.saveWindowSize(Qt.size(width, height))
    }
}
