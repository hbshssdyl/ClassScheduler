import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle {
    id: root

    property var operateMode
    property var controller

    color: "#FFFFFF"
    radius: 5

    border {
        color: "#D6D6D6"
        width: 1
    }

    ListView {
        id: listView

        model: controller.actionItemsList
        anchors.fill: parent
        //orientation: ListView.Horizontal
        //layoutDirection: Qt.LeftToRight
        spacing: 12

        delegate: SearchTeacherInfoDelegate{
            id: actionItem

            width: root.width
            height: 50
        }
    }
}
