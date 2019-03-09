import QtQuick 2.0

import "../CustomComponents" as CoreItems

CoreItems.ClickableItem {
    id: root
    width: 150
    height: 50
    radius: 5
    background.color: !enabled ? disabledBgColor : backgroundColor
    dropShadow.visible: true
    dropShadow.antialiasing: true
    dropShadow.radius: 12
    dropShadow.samples: 25
    dropShadow.transparentBorder: true
    property alias text: buttonText
    property alias icon: icon
    property bool checkable: false
    property bool checked: false
    property string disabledTxtColor: "#000000"
    property string textColor: "#ffffff"
    property string backgroundColor: "#bfbfbf"
    Rectangle {
        id: clickedLayer
        anchors.fill: parent
        color: "#000000"
        opacity: 0.1
        visible: false
        radius: root.radius
        z: 0
    }
    Image {
        id: icon
        visible: false
        anchors.left: root.left
        anchors.leftMargin: 10
        anchors.top: root.top
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
        id: buttonText
        text: "Button"
        anchors.left: icon.visible ? icon.right : root.left
        anchors.leftMargin: 5
        anchors.top: root.top
        width: icon.visible ? (root.width - icon.width - icon.anchors.leftMargin - (anchors.leftMargin * 2)) : (root.width - (anchors.leftMargin * 2))
        height: root.height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.Wrap
        color: root.enabled ? root.textColor : root.disabledTxtColor
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
