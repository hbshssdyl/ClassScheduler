import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import "../JSUtils/ColorUtils.js" as ColorUtils
import "../JSUtils/MainUtils.js" as MainUtils


Rectangle{
    id: delegateroot

    property int infoHeight: 60
    property int chartHeight: 300
    property int delegateHeight: infoHeight + chartHeight * 3
    property int rootWidth

    color: "transparent"
    radius: 5
    height: delegateHeight

    border {
        color: ColorUtils.getActionItemBorderColor()
        width: 1
    }

    ColumnLayout {
        id: rootLayout

        anchors.fill: parent
        spacing: 0

        Rectangle{
            id: infoRoot

            color: index % 2 == 0 ? "transparent" : "#DDDDDD"
            radius: 5

            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.preferredHeight: infoHeight

            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            Row{
                id: teacherInfo

                Repeater {
                    id: repeater

                    model: modelData.basicInfo

                    delegate: Rectangle{
                        id: teacherInfoItem

                        color: "transparent"
                        width: MainUtils.getTeacherInfoWidth(rootWidth, index, controller.teacherInfoMap.teacherInfoHeader.length)
                        height: infoHeight

                        border {
                            color: ColorUtils.getActionItemBorderColor()
                            width: 1
                        }

                        TextEdit{
                            id: teacherInfoText

                            selectByMouse: true
                            readOnly: true
                            anchors.centerIn: parent
                            width: parent.width
                            text: modelData
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: 5
                            rightPadding: 5
                            wrapMode: TextEdit.WordWrap
                            font {
                                bold: false
                                pixelSize: 12
                            }
                        }
                    }
                }
            }
        }
        Column {
            id: chartViewColumn
            spacing: 2

            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.preferredHeight: chartHeight * 3

            Repeater {
                id: chartViewRepeater

                model: modelData.studentInfo

                delegate: Rectangle{
                    id: chartRoot

                    color: "transparent"
                    radius: 5
                    height: chartHeight
                    width: parent.width

                    ChartView {
                        //title: "物理学生人数变化趋势图"
                        anchors.fill: parent
                        legend.visible: false
                        antialiasing: true

                        ListModel {
                            id: pointModel
                            ListElement { x: 1; y: 4 }
                            ListElement { x: 2; y: 4 }
                            ListElement { x: 3; y: 4 }
                            ListElement { x: 4; y: 3 }
                            ListElement { x: 5; y: 5 }
                        }

                        LineSeries {
                            id: series
                            // Y轴
                            pointLabelsVisible: true
                            pointLabelsFont.pixelSize: 15 // 只设置 pixelSize
                            pointLabelsFormat: "@yPoint"

                            axisY: ValuesAxis {
                                min: 0
                                max: 10
                                tickType: ValuesAxis.TicksFixed
                                tickInterval: 1
                                labelFormat: "%d"
                                titleText: "学生人数"
                            }
                            // X轴：一月到十二月
                            axisX: BarCategoryAxis {
                                // min: 1
                                // max: 12
                                //titleText: "月份"
                                //labelsPosition: CategoryAxis.AxisLabelsPositionOnValue
                                categories: ["总数", "投入", "产出", "欠数"];
                            }

                            Component.onCompleted: {
                                for (let i = 0; i < pointModel.count; ++i) {
                                    let item = pointModel.get(i);
                                    series.append(item.x, item.y);
                                }
                            }
                            // 其余月份可补充数据
                        }
                    }
                }
            }


        }

    }
}
