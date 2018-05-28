import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0

Item {
    id: root
    property string railColor: root.checked == true ? root.checkedColor : root.uncheckedColor
    property string textColor: "#ffffff"
    property string labelColor: "#000000"
    property string leftTextColor: root.textColor
    property string rightTextColor: root.textColor
    property int borderRadius: 0
    property string rightText: ""
    property string leftText: ""
    property string state: root.leftStateValue
    property bool checked: root.state == rightStateValue ? true : false
    property string leftStateValue: "left"
    property string rightStateValue: "right"
    property string checkedColor: "#34ad32"
    property string uncheckedColor: "#666666"
    property string font: "Arial"
    property int switchRailWidth: 100
    property int switchRailHeight: 30
    property string switchLabel: ""
    RowLayout {
        id: switchLayout
        anchors.fill: parent
        spacing: 5
        Text {
            id: labelText
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: 10
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            color: root.labelColor
            text: root.switchLabel
        }
        Rectangle {
            id: switchRail
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.preferredWidth: root.switchRailWidth
            Layout.preferredHeight: root.switchRailHeight
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            Layout.rightMargin: 10
            color: root.railColor
            radius: root.borderRadius
            antialiasing: true
            Rectangle {
                id: switchRailBorder
                anchors.top: parent.top
                anchors.topMargin: 1
                anchors.left: parent.left
                anchors.leftMargin: 1
                width: parent.width - 2
                height: parent.height - 2
                border.color: "gray"
                border.width: 1
                color: "transparent"
                Text {
                    id: leftText
                    anchors.left: parent.left
                    height: switchRailBorder.height
                    width: switchRailBorder.width / 2
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                    text: root.leftText
                    color: root.leftTextColor
                    font.family: root.font
                }
                Text {
                    id: rightText
                    anchors.right: parent.right
                    height: switchRailBorder.height
                    width: switchRailBorder.width / 2
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                    text: root.rightText
                    color: root.rightTextColor
                    font.family: root.font
                }
                Rectangle {
                    id: switchButton
                    anchors.top: switchRailBorder.top
                    anchors.topMargin: (switchRailBorder.height - switchButton.height) / 2
                    height: switchRailBorder.height
                    width: (switchRailBorder.width / 2)
                    border.width: 1
                    border.color: "#BBBBBB"
                    state: root.state
                    states: [
                        State {
                            name: root.rightStateValue
                            PropertyChanges {
                                target: switchButton
                                x: (switchRailBorder.width / 2)
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
                        source: parent
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "#bcbcbc" }
                            GradientStop { position: 1.0; color: "#cccccc" }
                        }
                    }
                }
                MouseArea {
                    id: mouse
                    anchors.fill: parent
                    onClicked: root.state = ((root.state == root.leftStateValue) ? root.rightStateValue : root.leftStateValue)
                }
            }
        }
    }
}
