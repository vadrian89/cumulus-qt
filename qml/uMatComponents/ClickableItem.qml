import QtQuick 2.0
import "." as CoreItems

CoreItems.Item {
    id: root
    property double hoveredBackgroundOpacity: backgroundOpacity
    property double unhoveredBackgroundOpacity: backgroundOpacity
    property bool ripple: false
    property int rippleDuration: 300
    property bool containsMouse: mouseArea.containsMouse
    signal clicked()
    signal entered()
    signal exited()
    signal pressed()
    signal released()
    Rectangle {
        id: itemRipple
        x: (root.width / 2)
        y: (root.height / 2)
        width: 0
        height: 0
        radius: (root.width / 2)
        visible: root.ripple
        color: "#000000"
        opacity: 0.1
        ParallelAnimation {
            id: rippleAnim
            NumberAnimation {
                target: itemRipple
                property: "radius"
                from: (root.width / 2)
                to: root.radius
                duration: root.rippleDuration
            }
            NumberAnimation {
                id: xAnim
                target: itemRipple
                property: "x"
                from: (root.width / 2)
                to: 0
                duration: root.rippleDuration
            }
            NumberAnimation {
                id: yAnim
                target: itemRipple
                property: "y"
                from: (root.height / 2)
                to: 0
                duration: root.rippleDuration
            }
            NumberAnimation {
                target: itemRipple
                property: "width"
                from: 0
                to: root.width
                duration: root.rippleDuration
            }
            NumberAnimation {
                target: itemRipple
                property: "height"
                from: 0
                to: root.height
                duration: root.rippleDuration
            }
        }
        PropertyAnimation on opacity {
            id: hideAnim
            from: 0.1
            to: 0.0
            running: false
        }
        Timer {
            id: resetRippleTimer
            interval: 1000
            repeat: false
            running: false
            onTriggered: hideAnim.restart()
        }
    }
    MouseArea {
        id: mouseArea
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
            xAnim.from = mouseX
            yAnim.from = mouseY
            resetRippleTimer.stop()
            itemRipple.opacity = 0.1
            rippleAnim.running = true
            parent.pressed()
        }
        onReleased: {
            if (rippleAnim.running) resetRippleTimer.restart()
            else hideAnim.restart()
            parent.released()
        }
        onContainsMouseChanged: {
            if (!containsMouse)
                parent.backgroundOpacity = parent.unhoveredBackgroundOpacity
            else
                parent.backgroundOpacity = parent.hoveredBackgroundOpacity
        }
    }
}
