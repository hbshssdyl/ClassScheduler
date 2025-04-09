import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle{
    id: root

    property int minDelegateHeight
    property int delegateHeight: minDelegateHeight * teacherClassItemView.count

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
            id: teacherNameItem
            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: 60
            Layout.preferredHeight: root.height
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            Text{
                id: teacherName

                anchors.centerIn: parent
                text: modelData.teacherName
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: teacherClassItem
            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true
            Layout.preferredHeight: root.height
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            ListView {
                id: teacherClassItemView

                model: modelData.teacherCourseList
                anchors.fill: parent

                delegate: TeacherCourseItemDelegate{
                    id: delegateItem

                    width: teacherClassItem.width
                    minDelegateHeight: root.minDelegateHeight
                    height: minDelegateHeight
                }
            }
        }

    }
}
