import QtQuick 2.0

SimpleItem {
    property double hoveredBackgroundOpacity: backgroundOpacity
    property double unhoveredBackgroundOpacity: backgroundOpacity
    signal clicked()
    signal entered()
    signal exited()
    signal pressed()
    signal released()
    signal containsMouseChanged(bool containsMouse)
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: parent.clicked()
        onEntered: {
            parent.backgroundOpacity = parent.hoveredBackgroundOpacity
            parent.entered()
        }
        onExited: {
            parent.backgroundOpacity = parent.unhoveredBackgroundOpacity
            parent.exited()
        }
        onPressed: {
            parent.backgroundOpacity = parent.hoveredBackgroundOpacity
            parent.pressed()
        }
        onReleased: {
            parent.backgroundOpacity = parent.unhoveredBackgroundOpacity
            parent.released()
        }
        onContainsMouseChanged: {
            parent.containsMouseChanged(containsMouse)
            if (!containsMouse)
                parent.backgroundOpacity = parent.unhoveredBackgroundOpacity
            else
                parent.backgroundOpacity = parent.hoveredBackgroundOpacity
        }
    }
}
