import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15

Rectangle {
    id: root

    property var operateMode
    property var rootController

    color: "#FFFFFF"
    radius: 5

    border {
        color: "#D6D6D6"
        width: 1
    }

    RowLayout{
        id: rowLayout
        anchors.fill: parent
        spacing: 0

        Rectangle{
            id: leftArea

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: parent.width / 2 - 10
            Layout.fillHeight: true
            Layout.margins: 5
            color: "#FFFFFF"
            radius: 5

            border {
                color: "#D6D6D6"
                width: 1
            }

        }
        Rectangle{
            id: rightArea

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: parent.width / 2 - 10
            Layout.fillHeight: true
            Layout.margins: 5
            color: "#FFFFFF"
            radius: 5

            border {
                color: "#D6D6D6"
                width: 1
            }

        }
    }
}
