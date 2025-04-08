import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle{
    id: root

    property var selectedOperateMode

    color: "transparent"
    radius: 5

    border {
        color: ColorUtils.getActionItemBorderColor()
        width: 1
    }

    RowLayout {
        id: rowLayout

        anchors.fill: parent
        spacing: 0

        Rectangle{
            id: courseTypeItem
            color: "transparent"

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            Layout.preferredHeight: root.height
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            Text{
                id: courseType

                anchors.centerIn: parent
                text: "名字"
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: courseNameItem
            color: "transparent"

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            Layout.preferredHeight: root.height
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            Text{
                id: courseName

                anchors.centerIn: parent
                text: "名字"
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: coursePricesItem

            color: "transparent"

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            Layout.preferredHeight: root.height
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            Text{
                id: coursePrices

                anchors.centerIn: parent
                text: "50"
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }
    }
}
