import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle{
    id: root

    property var selectedOperateMode

    color: modelData.isSelected ? ColorUtils.getActionItemBackgroundSelectedColor() : ColorUtils.getActionItemBackgroundUnselectedColor()
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
            id: teacherNameItem
            color: "transparent"

            Layout.alignment: Qt.AlignCenter
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            Text{
                id: teacherName

                anchors.centerIn: parent
                text: "名字"
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }
    }
}
