import QtWebEngine 1.2
import QtQuick 2.5

WebEngineView {
    id: root
    property string systemLanguage: Qt.locale().name.substring(0,2)
    url: "data/app.html?lang=" + systemLanguage
    signal disableDrag()
    signal enableDrag()
    signal appOpacityChanged(double opacity)
    signal trayIconChanged(string iconText)
    signal closeApp()
    signal hideApp()
    signal minimizeApp()
    signal changeTrayVisibility()

    Component.onCompleted: {
        runJavaScript(util.launcherCountVisibility())
    }

    onJavaScriptConsoleMessage: {
        console.log("Cumulus javascript log: " + message)
        if(message.substring(0,3) == "url") {
            Qt.openUrlExternally(message.substring(3,message.length))
        }
    }

    onTitleChanged: {
        console.log("Cumulus title changed: " + title)
        if (title == "disabledrag") {
            root.disableDrag()
        }
        else if (title == "enabledrag") {
            root.enableDrag()
        }
        else if (title.substring(0,1) == "o") {
            root.appOpacityChanged(Number(title.substring(1,5)))
        }
        else if (title == "enable_launcher" || title == "disable_launcher") {
            util.toggleLauncherCount(title)
        }
        else if (title.substring(0,1) == "t") {
            util.setLauncherCount(title.substring(1,title.length))
            root.trayIconChanged(title.substring(1,title.length))
        }
        else if (title == "close") {
            if (util.applicationVisiblity() === false)
                root.closeApp()
            else
                root.hideApp()

        }
        else if (title == "minimize") {
            root.minimizeApp()
        }
        else if (title == "enable_tray" || title == "disable_tray") {
            util.saveApplicationVisibility(title)
            root.changeTrayVisibility()
        }
    }
}
