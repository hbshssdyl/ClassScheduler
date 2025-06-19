import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import "../JSUtils/ColorUtils.js" as ColorUtils
import "../JSUtils/MainUtils.js" as MainUtils

Rectangle {
    id: delegateroot

    property int maxStudentCount
    property int infoHeight: 60
    property int chartHeight: 300
    property int chartCount: modelData.studentInfo.length
    property int delegateHeight: infoHeight + chartHeight * chartCount
    property int rootWidth
    property var monthLabels: modelData.studentInfo.yearMonthList

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

        Rectangle {
            id: infoRoot

            color: "transparent"
            radius: 5

            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.preferredHeight: infoHeight

            border {
                color: ColorUtils.getActionItemBorderColor()
                width: 1
            }

            Row {
                id: teacherInfo

                Repeater {
                    id: repeater

                    model: modelData.basicInfo

                    delegate: Rectangle {
                        id: teacherInfoItem

                        color: "transparent"
                        width: MainUtils.getTeacherInfoWidth(rootWidth, index, controller.teacherInfoMap.teacherInfoHeader.length)
                        height: infoHeight

                        border {
                            color: ColorUtils.getActionItemBorderColor()
                            width: 1
                        }

                        TextEdit {
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

                            Component.onCompleted: {
                                if(teacherInfoText.height + 10 > infoHeight)
                                {
                                    infoHeight = teacherInfoText.height + 10;
                                }
                            }
                        }
                    }
                }
            }
        }
        Column {
            id: chartViewColumn
            spacing: 0

            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.preferredHeight: chartHeight * chartCount

            Repeater {
                id: chartViewRepeater

                model: modelData.studentInfo

                delegate: Rectangle {
                    id: chartRoot

                    color: "transparent"
                    radius: 5
                    height: chartHeight
                    width: parent.width

                    ChartView {
                        anchors.fill: parent
                        legend.visible: false
                        antialiasing: true

                        LineSeries {
                            id: series
                            pointLabelsVisible: true
                            pointLabelsFont.pixelSize: 15
                            pointLabelsFormat: "@yPoint"
                            pointLabelsClipping: false  // 允许标签超出图表区域

                            axisY: ValuesAxis {
                                min: 0
                                max: maxStudentCount
                                tickType: ValuesAxis.TicksFixed
                                tickInterval: 1
                                labelFormat: "%d"
                                titleText: modelData.suject + "学生人数"
                            }

                            axisX: CategoryAxis {
                                min: 1
                                max: modelData.yearMonthList.length
                                labelsPosition: CategoryAxis.AxisLabelsPositionOnValue

                                // 动态创建 CategoryRange
                                Component.onCompleted: {
                                    for (var i = 0; i < modelData.yearMonthList.length; i++) {
                                        append(modelData.yearMonthList[i], i + 1);
                                    }
                                }
                            }

                            Component.onCompleted: {
                                for (let i = 0; i < modelData.monthCountList.length; ++i) {
                                    let item = modelData.monthCountList[i];
                                    series.append(i + 1, item);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
