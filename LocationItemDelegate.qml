import QtQuick 2.0

ClickableItem {
    id: root
    property int locationId: 0
    property alias text: delegateText.text
    property alias textColor: delegateText.color
    property alias textFontFamily: delegateText.font.family
    property alias fontSize: delegateText.font.pixelSize
    property bool isSelected: false
    signal clickedDelete()

    backgroundOpacity: (root.isSelected) ? 0.4 : 0.2
    hoveredBackgroundOpacity: 0.4
    unhoveredBackgroundOpacity: (root.isSelected) ? 0.4 : 0.2
    radius: 2
    Image {
        id: selectedImage
        anchors.right: left.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        height: root.height - 20
        width: selectedImage.height
        sourceSize.height: selectedImage.height
        sourceSize.width: selectedImage.width
        source: "image://fontimage/\uf00c" + (root.textColor ? root.textColor : "#ffffff")
        visible: root.isSelected
    }
    Text {
        id: delegateText
        anchors.left: selectedImage.right
        anchors.leftMargin: 10
        anchors.top: parent.top
        width: (parent.width - selectedImage.width - deleteButton.width)
        height: parent.height
        verticalAlignment: Text.AlignVCenter
    }
    ClickableItem {
        id: deleteButton
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        width: deleteButton.height
        height: root.height - 20
        visible: true
        backgroundOpacity: 0.0
        hoveredBackgroundOpacity: backgroundOpacity
        unhoveredBackgroundOpacity: backgroundOpacity
        Image {
            anchors.fill: parent
            sourceSize.height: selectedImage.height
            sourceSize.width: selectedImage.width
            source: "image://fontimage/\uf1f8" + (root.textColor ? root.textColor : "#ffffff")
        }
        onClicked: root.clickedDelete()
    }
}
