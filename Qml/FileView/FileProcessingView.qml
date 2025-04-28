import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.platform

Rectangle {
    id: mainContainer

    property var rootController

    anchors.fill: parent
    color: "#e8f5fe" // 柔和的背景色

    Rectangle {
        id: contentBox
        width: parent.width * 0.9
        height: parent.height * 0.6
        anchors.centerIn: parent
        radius: 10
        color: "#ffffff" // 白色背景
        border.color: "#b0bec5" // 边框颜色
        border.width: 2
        //elevation: 2

        Column {
            id: contentColumn
            anchors.centerIn: parent
            spacing: 20
            width: parent.width * 0.8

            Text {
                id: fileNameText
                text: "Processing File: text.qml"
                font.pixelSize: 18
                font.bold: true
                color: "#37474f" // 深灰色字体
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Rectangle {
                id: loaderContainer
                width: 80
                height: 80
                radius: 40
                color: "#e3f2fd" // 浅蓝色背景
                border.color: "#90caf9" // 边框颜色
                border.width: 2
                anchors.horizontalCenter: parent.horizontalCenter

                BusyIndicator {
                    id: busyIndicator
                    running: true
                    anchors.fill: parent
                }
            }
        }
    }
}
