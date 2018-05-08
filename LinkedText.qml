import QtQuick 2.0

Text {
    id: root
    property string url
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: Qt.openUrlExternally(root.url)
    }
}
