import QtQuick 2.0
import "." as CoreItems

CoreItems.Item {
    id: root
    property double hoveredBackgroundOpacity: backgroundOpacity
    property double unhoveredBackgroundOpacity: backgroundOpacity
    property bool ripple: false
    property int rippleDuration: 250
    property int rippleHideDuration: 300
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
        opacity: 0.2
        SequentialAnimation {
            id: rippleAnim
            ParallelAnimation {
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
            NumberAnimation {
                target: itemRipple
                property: "opacity"
                from: 0.2
                to: 0.0
                duration: root.rippleHideDuration
            }
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
            itemRipple.width = 0
            itemRipple.height = 0
            itemRipple.opacity = 0.2
            rippleAnim.restart()
            parent.pressed()
        }
        onReleased: {
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
