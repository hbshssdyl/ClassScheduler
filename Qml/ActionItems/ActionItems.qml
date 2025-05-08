import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15

Rectangle{
    id: root

    property var controller
    color: "transparent"
    clip: true


    ListView {
        id: listView

        model: controller.actionItemsList
        anchors.fill: parent
        orientation: ListView.Horizontal
        layoutDirection: Qt.LeftToRight
        spacing: 12

        delegate: ActionItemDelegate{
            id: actionItem

            width: 100
            height: 50

            onButtonClicked: function(mode){
                controller.onOperateModeSelected(mode);
            }
        }
    }
}


