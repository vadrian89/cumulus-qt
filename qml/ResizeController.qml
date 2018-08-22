import QtQuick 2.0

Item {
    id: root
    property int cursorShape: Qt.SizeHorCursor
    property int xResize: 0
    property int yResize: 0

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        cursorShape: root.cursorShape
        property variant clickPos
        onPressed: clickPos  = Qt.point(mouse.x,mouse.y)
        onPositionChanged: {
            var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
            xResize = delta.x
            yResize = delta.y
        }
    }
}
