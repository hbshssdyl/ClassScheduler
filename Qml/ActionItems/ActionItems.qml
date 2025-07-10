import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle {
    id: root
    property var controller
    color: "transparent"
    clip: true

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: controller.actionItemsList
            //orientation: ListView.Horizontal
            spacing: 12

            delegate: ActionItemDelegate {
                width: root.width
                height: 43
                onButtonClicked: controller.onOperateModeSelected(mode)
            }
        }
    }
}
