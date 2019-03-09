import QtQuick 2.0

CustomButton {
    id: root
    dropShadow.visible: false
    radius: 0
    textColor: "#000000"
    backgroundColor: "#ffffff"
    text.text: "Setting"
    text.height: summaryText.visible ? ((root.height / 2) - 5) : root.height
    text.horizontalAlignment: Text.AlignLeft
    text.verticalAlignment: summaryText.visible ? Text.AlignBottom : Text.AlignVCenter
    icon.height: 35
    icon.width: 35
    property alias summaryText: summaryText
    Text {
        id: summaryText
        text: "Summary"
        color: "#606060"
        width: root.text.width
        height: root.text.height
        anchors.left: root.text.anchors.left
        anchors.leftMargin: root.text.anchors.leftMargin
        anchors.top: root.text.bottom
        anchors.topMargin: 5
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignTop
        font.pixelSize: (root.text.font.pixelSize - 2)
        wrapMode: Text.NoWrap
    }
}
