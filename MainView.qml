import QtQuick 2.0

Item {
    id: root
    property alias tempValue: weatherView.tempValue
    property int contentWidth: ((root.width < 600) ? (root.width * 2) : root.width)
    property int viewWidth: ((root.width < 600) ? root.width : (root.width / 2))
    property int startPoint: ((root.width < 600) ? root.viewWidth : 0)
    property string textColor
    property string textFontFamily
    property string iconsFont
    property string speedUnit
    property string tempUnit
    property string pressureUnit
    property int widthBreakPoint
    property string backgroundColor
    signal locationNameChanged(string name)
    signal finishedWeatherUpdate()
    signal updateWeather()
    signal networkError()
    signal loadLogoImage()

    onTextColorChanged: weatherView.loadLogoImage()
    onLoadLogoImage: weatherView.loadLogoImage()
    onUpdateWeather: weatherView.updateWeather()

    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: root.contentWidth
        contentHeight: root.height
        interactive: true
        contentX: root.startPoint
        LocationView {
            id: locationView
            anchors.left: parent.left
            anchors.top: parent.top
            width: root.viewWidth
            height: parent.height
            backgroundOpacity: 0.0
            backgroundColor: root.backgroundColor
            textColor: root.textColor
            textFontFamily: root.textFontFamily
            visible: true
            onNoLocation: weatherView.visible = false
        }
        WeatherWindow {
            id: weatherView
            anchors.left: locationView.right
            anchors.top: parent.top
            width: root.viewWidth
            height: parent.height
            textColor: root.textColor
            textFontFamily: root.textFontFamily
            iconsFont: root.iconsFont
            speedUnit: root.speedUnit
            widthBreakPoint: root.widthBreakPoint
            tempUnit: root.tempUnit
            pressureUnit: root.pressureUnit
            onFinishedWeatherUpdate: root.finishedWeatherUpdate()
            onNetworkError: root.networkError()
            onLocationNameChanged: root.locationNameChanged(locationName)
        }
        onMovementEnded: {
            if (contentX > (root.viewWidth / 2))
                flickable.contentX = root.startPoint
            else
                flickable.contentX = 0
        }
    }
    onWidthChanged: {
        if (flickable.contentX != 0)
            flickable.contentX = root.viewWidth
    }
    onRotationChanged: {
        if (contentX > (root.viewWidth / 2))
            flickable.contentX = root.startPoint
        else
            flickable.contentX = 0
    }
}
