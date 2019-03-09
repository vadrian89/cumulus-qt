import QtQuick 2.0

import "../CustomComponents" as CoreItems

CoreItems.ClickableItem {
    id: root
    width: 150
    height: 50
    radius: 5
    backgroundColor: "#bfbfbf"
    background.color: !enabled ? disabledBgColor : backgroundColor
    dropShadow.visible: true
    dropShadow.antialiasing: true
    dropShadow.radius: 12
    dropShadow.samples: 25
    dropShadow.transparentBorder: true
    property string textColor: "#ffffff"
    property bool checkable: false
    property bool checked: false
    property alias icon: icon
    property alias text: text
    Rectangle {
        id: clickedLayer
        anchors.fill: parent
        color: "#000000"
        opacity: 0.1
        visible: false
        radius: parent.radius
        z: 0
    }
    Image {
        id: icon
        visible: false
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: ((root.height / 2) - (height / 2))
        width: height
        height: (root.height * 0.6)
        sourceSize.width: width
        sourceSize.height: height
        horizontalAlignment: Image.AlignHCenter
        verticalAlignment: Image.AlignVCenter
        z: 1
    }
    Text {
        id: text
        visible: (text.text.trim().length > 0)
        anchors.left: icon.visible ? icon.right : parent.left
        anchors.leftMargin: 5
        anchors.top: parent.top
        width: (text.visible && icon.visible) ? (root.width - icon.width - icon.anchors.leftMargin - anchors.leftMargin * 2) : (root.width - anchors.leftMargin * 2)
        height: root.height
        font.pixelSize: 14
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: root.enabled ? root.textColor : root.disabledTxtColor
        z: 2
    }
    onPressed: {
        if (!ripple)
            clickedLayer.visible = true
    }
    onReleased: {
        if (!ripple)
            clickedLayer.visible = false
        if (checkable && containsMouse)
            root.checked = !root.checked
    }
}
