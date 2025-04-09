import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle{
    id: root

    property int minDelegateHeight
    property int textItemWidth: 100

    color: "transparent"
    radius: 5

    border {
        color: "red"
        width: 1
    }

    RowLayout {
        id: rowLayout

        anchors.fill: parent
        spacing: 0

        Rectangle{
            id: sujectItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.minDelegateHeight
            border {
                color: "yellow"
                width: 1
            }

            Text{
                id: suject

                anchors.centerIn: parent
                text: modelData.suject
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: dateItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.minDelegateHeight
            border {
                color: "blue"
                width: 1
            }

            Text{
                id: date

                anchors.centerIn: parent
                text: modelData.date
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: timeItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.minDelegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            Text{
                id: time

                anchors.centerIn: parent
                text: modelData.time
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: weekItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.minDelegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            Text{
                id: week

                anchors.centerIn: parent
                text: modelData.week
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: schoolItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.minDelegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            Text{
                id: school

                anchors.centerIn: parent
                text: modelData.school
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: gradeItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.minDelegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            Text{
                id: grade

                anchors.centerIn: parent
                text: modelData.grade
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: courseTimeItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.minDelegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            Text{
                id: courseTime

                anchors.centerIn: parent
                text: modelData.courseTime
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: studentFeeItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.minDelegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            Text{
                id: studentFee

                anchors.centerIn: parent
                text: modelData.studentFee
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: teacherFeeItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.minDelegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            Text{
                id: teacherFee

                anchors.centerIn: parent
                text: modelData.teacherFee
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Item {
            id: spaceItem

            Layout.fillWidth: true
        }

    }
}
