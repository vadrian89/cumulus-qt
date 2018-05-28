import QtQuick 2.0

ClickableItem {
    id: root
    property string forecastDay
    property string forecastIcon
    property string forecastTempHigh
    property string forecastTempLow
    property string forecastDesc
    property string stringFont
    property string iconFont
    property string fontColor
    unhoveredBackgroundOpacity: 0.2
    hoveredBackgroundOpacity: 0.4
    backgroundOpacity: unhoveredBackgroundOpacity
    property int widthBreakPoint: 0

    Text {
        id: icon
        width: height
        height: parent.height
        anchors.top: parent.top
        anchors.left: parent.left
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.family: iconFont
        font.pixelSize: 20
        color: fontColor
        text: root.forecastIcon
        visible: (parent.width > parent.widthBreakPoint)
    }
    Text {
        id: date
        width: (icon.visible) ? (parent.width - icon.width - tempHigh.width - tempLow.width) : (parent.width - anchors.leftMargin - tempHigh.width - tempLow.width)
        height: (parent.height * 50 / 100)
        anchors.top: parent.top
        anchors.left: (icon.visible) ? icon.right : parent.left
        anchors.leftMargin: (icon.visible) ? 0 : 10
        color: fontColor
        font.family: stringFont
        font.pixelSize: 18
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        text: qsTr(util.forecastDate(root.forecastDay))
    }
    Text {
        id: description
        width: date.width
        height: (parent.height - date.height)
        anchors.top: date.bottom
        anchors.left: date.anchors.left
        anchors.leftMargin: date.anchors.leftMargin
        color: fontColor
        font.family: stringFont
        font.pixelSize: 14
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        text: qsTr(root.forecastDesc)
    }
    Text {
        id: tempHigh
        width: 40
        height: width
        anchors.top: parent.top
        anchors.topMargin: ((parent.height / 2) - (height / 2))
        anchors.left: description.right
        color: fontColor
        font.family: stringFont
        font.pixelSize: 20
        font.weight: Font.DemiBold
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        text: root.forecastTempHigh + " / "
    }
    Text {
        id: tempLow
        width: tempHigh.width
        height: tempHigh.height
        anchors.top: parent.top
        anchors.topMargin: tempHigh.anchors.topMargin
        anchors.left: tempHigh.right
        color: fontColor
        font.family: tempHigh.font.family
        font.pixelSize: (tempHigh.font.pixelSize - 4)
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        text: root.forecastTempLow
    }
}
