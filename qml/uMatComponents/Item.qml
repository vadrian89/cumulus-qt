import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: root
    property string backgroundColor: "#ffffff"
    property string disabledBgColor: "#a8a8a8"
    property string disabledTxtColor: "#000000"
    property double backgroundOpacity: 1.0
    property int radius: 0
    property int borderWidth: 0
    property string borderColor: "transparent"
    property alias dropShadow: dropShadow
    property alias background: background
    Rectangle {
        id: background
        anchors.fill: parent
        color: root.enabled ? root.backgroundColor : root.disabledBgColor
        opacity: root.backgroundOpacity
        radius: root.radius
        border.width: root.borderWidth
        border.color: root.borderColor
    }
    DropShadow {
        id: dropShadow
        visible: false
        source: background
        anchors.fill: background
        color: "#494949"
    }
}
