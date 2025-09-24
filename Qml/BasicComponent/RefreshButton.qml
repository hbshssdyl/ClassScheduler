import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Rectangle {
    id: root
    property alias iconSource: icon.source
    property color buttonColor: "white"       // 橙色按钮底色
    property color glowColor: "white"         // 光效颜色，亮橙色
    property int iconSize: 24
    property int busyIndicatorSize: 40
    signal clicked()
    signal finished()

    width: 26
    height: 26
    radius: width / 2
    color: buttonColor
    layer.enabled: true

    Image {
        id: icon
        anchors.centerIn: parent
        sourceSize: Qt.size(root.iconSize, root.iconSize)
        visible: true
    }

    BusyIndicator {
        id: spinner
        anchors.centerIn: parent
        width: root.busyIndicatorSize
        height: root.busyIndicatorSize
        running: false
        visible: false
    }

    layer.effect: Glow {
        samples: 16
        color: glowColor
        spread: 0.5
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            icon.visible = false
            spinner.visible = true
            spinner.running = true
            root.clicked()
        }
    }

    onFinished: {
        spinner.running = false
        spinner.visible = false
        icon.visible = true
    }
}
