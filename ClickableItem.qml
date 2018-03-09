import QtQuick 2.0

SimpleItem {
    property double hoveredBackgroundOpacity: 1.0
    property double unhoveredBackgroundOpacity: 1.0
    signal clicked()
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: parent.clicked()
        onEntered: parent.backgroundOpacity = parent.hoveredBackgroundOpacity
        onExited: parent.backgroundOpacity = parent.unhoveredBackgroundOpacity
        onPressed: parent.backgroundOpacity = parent.hoveredBackgroundOpacity
        onReleased: parent.backgroundOpacity = parent.unhoveredBackgroundOpacity
        onContainsMouseChanged: {
            if (!containsMouse)
                parent.backgroundOpacity = parent.unhoveredBackgroundOpacity
            else
                parent.backgroundOpacity = parent.hoveredBackgroundOpacity
        }
    }
}
