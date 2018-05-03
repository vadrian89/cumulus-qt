import QtQuick 2.0
import QtQuick.Controls 2.0

import ownTypes.LocationController 0.1

SimpleItem {
    id: root
    property string textColor: "#ffffff"
    property string textFontFamily: "Arial"
    property alias locationController: locationController
    signal noLocation()
    ListView {
        id: locationList
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: parent.width - 20
        height: (parent.height - addLocationButton.height - 20)
        orientation: ListView.Vertical
        snapMode: ListView.SnapToItem
        clip: true
        spacing: 2
        delegate: LocationItemDelegate {
            locationId: model.modelData.locationId
            text: model.modelData.locationName
            textColor: root.textColor
            textFontFamily: root.textFontFamily
            width: parent.width
            height: 50
            isSelected: model.modelData.isCurrentLocation
            onClicked: {
                if (!isSelected) {
                    locationController.changeCurrentLocation(locationId)
                }
            }
            onClickedDelete: {
                if (!isSelected)
                    locationController.deleteLocation(locationId)
            }
        }
        model: locationController.locationList
    }

    ClickableItem {
        id: addLocationButton
        width: 50
        height: width
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        backgroundOpacity: 1.0
        backgroundColor: "transparent"
        hoveredBackgroundOpacity: 0.6
        unhoveredBackgroundOpacity: 1.0
        borderColor: root.textColor
        borderWidth: 1
        radius: (width / 2)
        Image {
            anchors.centerIn: parent
            height: (parent.height * 70 / 100)
            width: height
            sourceSize.height: height
            sourceSize.width: width
            source: "image://fontimage/\uf067" + (root.textColor ? root.textColor : "#ffffff")
        }
        onClicked: {
            locationSearchLoader.source = "SearchLocation.qml"
            locationSearchLoader.visible = true
            locationController.getGpsLocation()
        }
    }

    Loader {
        id: locationSearchLoader
        visible: false
        Binding {
            when: (locationSearchLoader.visible)
            target: locationSearchLoader.item
            property: "backgroundColor"
            value: root.backgroundColor
        }
        Binding {
            when: (locationSearchLoader.visible)
            target: locationSearchLoader.item
            property: "textColor"
            value: root.textColor
        }
        Binding {
            when: (locationSearchLoader.visible)
            target: locationSearchLoader.item
            property: "textFontFamily"
            value: root.textFontFamily
        }
        Binding {
            when: (locationSearchLoader.visible)
            target: locationSearchLoader.item
            property: "width"
            value: root.width
        }
        Binding {
            when: (locationSearchLoader.visible)
            target: locationSearchLoader.item
            property: "height"
            value: root.height
        }
        Binding {
            when: (locationSearchLoader.visible)
            target: locationSearchLoader.item
            property: "searchListModel"
            value: locationController.locationSearchList
        }
        Binding {
            when: (locationSearchLoader.visible)
            target: locationSearchLoader.item
            property: "gpsLocation"
            value: locationController.gpsLocation
        }
        Binding {
            target: locationSearchLoader.item
            property: "visible"
            value: locationSearchLoader.visible
        }
        Connections {
            id: locationSearchCon
            target: locationSearchLoader.item
            onLocationQueryChanged: locationController.searchLocation(locationSearchLoader.item.locationQuery)
            onVisibleChanged: locationSearchLoader.visible = locationSearchLoader.item.visible
            onLocationSelected: {
                locationController.insertLocation(locationSelected)
                locationSearchLoader.visible = false                
            }
        }
    }
    LocationController {
        id: locationController
        onNoLocation: {
            root.noLocation()
            addLocationButton.clicked()
        }
    }
    Component.onCompleted: locationController.getLocationList()
}
