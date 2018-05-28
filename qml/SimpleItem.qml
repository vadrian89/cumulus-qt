import QtQuick 2.0

Item {
    id: root
    property string backgroundColor: "#ffffff"
    property double backgroundOpacity: 1.0
    property int radius: 0
    property int borderWidth: 0
    property string borderColor: "transparent"
    Rectangle {
        anchors.fill: parent
        color: root.backgroundColor
        opacity: root.backgroundOpacity
        radius: root.radius
        border.width: root.borderWidth
        border.color: root.borderColor
    }
}
