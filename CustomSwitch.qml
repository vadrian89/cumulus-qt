import QtQuick 2.7
import QtGraphicalEffects 1.0

Item {
    id: root
    property string railColor: "#161616"
    property string textColor: "#ffffff"
    property string leftTextColor: root.textColor
    property string rightTextColor: root.textColor
    property int borderRadius: 2
    property alias rightText: rightText.text
    property alias leftText: leftText.text
    property alias state: switchButton.state
    property bool checked: root.state == rightStateValue ? true : false
    property string leftStateValue: "left"
    property string rightStateValue: "right"
    Rectangle {
        id: switchRail
        anchors.fill: parent
        color: root.railColor
        radius: root.borderRadius
        antialiasing: true

        Rectangle {
            id: switchButton
            anchors.top: parent.top
            anchors.topMargin: (switchRail.height - switchButton.height) / 2
            height: switchRail.height
            width: (switchRail.width / 2)
            radius: root.borderRadius
            antialiasing: true
            states: [
                State {
                    name: root.rightStateValue
                    PropertyChanges {
                        target: switchButton
                        x: (switchRail.width / 2)
                    }
                },
                State {
                    name: root.leftStateValue
                    PropertyChanges {
                        target: switchButton
                        x: 0
                    }
                }
            ]
            transitions: Transition {
                NumberAnimation {
                    properties: "x"
                    easing.type: Easing.InOutBack
                }
            }
            RadialGradient {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#1c437a" }
                    GradientStop { position: 1.0; color: "#3e70b7" }
                }
            }
        }

        Text {
            id: leftText
            anchors.left: parent.left
            height: switchRail.height
            width: switchRail.width / 2
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true
            text: ""
            color: root.leftTextColor
        }

        Text {
            id: rightText
            anchors.right: parent.right
            height: switchRail.height
            width: switchRail.width / 2
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true
            text: ""
            color: root.rightTextColor
        }

        MouseArea {
            id: mouse
            anchors.fill: parent
            onClicked: root.state = root.state == root.leftStateValue ? root.rightStateValue : root.leftStateValue
        }
    }
}
