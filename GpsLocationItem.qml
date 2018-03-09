import QtQuick 2.0

ClickableItem {
    id: root
    property int locationId: 0
    property alias text: delegateText.text
    property alias textColor: delegateText.color
    property alias textFontFamily: delegateText.font.family
    property alias fontSize: delegateText.font.pixelSize

    backgroundOpacity: (root.isSelected) ? 0.4 : 0.2
    hoveredBackgroundOpacity: 0.4
    unhoveredBackgroundOpacity: (root.isSelected) ? 0.4 : 0.2
    radius: 2
    Image {
        id: iconImage
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 10
        height: root.height - 20
        width: iconImage.height
        sourceSize.height: iconImage.height
        sourceSize.width: iconImage.width
        source: "image://fontimage/\uf14e" + (root.textColor ? root.textColor : "#ffffff")
    }
    Text {
        id: delegateText
        anchors.left: iconImage.right
        anchors.leftMargin: 10
        anchors.top: parent.top
        width: (parent.width - iconImage.width)
        height: parent.height
        verticalAlignment: Text.AlignVCenter
    }
}
