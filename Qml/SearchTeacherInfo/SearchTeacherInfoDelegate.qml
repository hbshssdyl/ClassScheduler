import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle{
    id: root

    property int delegateHeight: 50
    property int textItemWidth: 100

    color: index % 2 == 0 ? "transparent" : "#DDDDDD"
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
            id: classInfoIdItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth / 2
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: classInfoId

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.id
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
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: date

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.date
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: weekendItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: weekend

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.weekend
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: studentNameItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: studentName

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.studentName
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
            Layout.preferredWidth: root.textItemWidth + 70
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: school

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.school
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: studentPhoneNubmerItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: studentPhoneNubmer

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.studentPhoneNubmer
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
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: grade

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.grade
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }


        Rectangle{
            id: sujectItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: suject

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.suject
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
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: time

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.time
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: teacherNickNameItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: teacherNickName

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.teacherNickName
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: learningTypeItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: learningType

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.learningType
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
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: courseTime

                selectByMouse: true
                readOnly: true
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
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: studentFee

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.studentFee
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }


        Rectangle{
            id: studentTotalFeeItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: studentTotalFee

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.studentTotalFee
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: teacherNameItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: teacherName

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.teacherName
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
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: teacherFee

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.teacherFee
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: gotMoneyItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: gotMoney

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.gotMoney
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: payTypeItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: payType

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.payType
                font {
                    bold: false
                    pixelSize: 12
                }
            }
        }

        Rectangle{
            id: payDateItem

            color: "transparent"

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: root.textItemWidth
            Layout.preferredHeight: root.delegateHeight
            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            TextEdit{
                id: payDate

                selectByMouse: true
                readOnly: true
                anchors.centerIn: parent
                text: modelData.payDate
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
