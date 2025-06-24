import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import "../JSUtils/ColorUtils.js" as ColorUtils
import "../JSUtils/MainUtils.js" as MainUtils

Rectangle {
    id: delegateroot

    property int rootWidth
    property var monthLabels: modelData.classInfo.yearMonthList
    property int maxClassCount
    property int infoHeight: 60
    property int chartHeight: 300
    property int toolHeight: 50
    property int chartMaxCount: modelData.classInfo.length
    property int chartCount: 1
    property int delegateHeight: infoHeight + toolHeight + chartHeight * chartCount


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
                id: studentInfo

                Repeater {
                    id: repeater

                    model: modelData.basicInfo

                    delegate: Rectangle {
                        id: studentInfoItem

                        color: "transparent"
                        width: MainUtils.getStudentInfoWidth(rootWidth, index, controller.studentInfoMap.studentInfoHeader.length)
                        height: infoHeight

                        border {
                            color: ColorUtils.getActionItemBorderColor()
                            width: 1
                        }

                        TextEdit {
                            id: studentInfoText

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
                                if(studentInfoText.height + 10 > infoHeight)
                                {
                                    infoHeight = studentInfoText.height + 10;
                                }
                            }
                        }
                    }
                }
            }
        }
        Item {
            id: advancedInfoItem

            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.preferredHeight: toolHeight + chartHeight * chartCount

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                Row {
                    id: advancedToolsItem

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop
                    Layout.preferredHeight: toolHeight

                    Repeater {
                        id: advancedToolsRepeater

                        anchors.centerIn: parent

                        model: [
                            { "key": "显示总课程数", "state": true,  "index": 0 },
                            { "key": "显示各科课程数", "state": false, "index": 1 },
                            { "key": "显示成绩曲线", "state": false, "index": 2 }
                        ]

                        delegate: Rectangle {
                            id: toolItemBg

                            width: MainUtils.getToolsItemWidth(rootWidth, index, advancedToolsRepeater.model.length)
                            height: toolHeight
                            // radius: 8
                            color: "#F5F5F5"
                            border.color: "#E0E0E0"
                            border.width: 1

                            Switch {
                                id: toolItem

                                anchors.centerIn: parent
                                text: modelData.key
                                checked: modelData.state
                                onClicked: {
                                    let data = modelData;
                                    let root = delegateroot;

                                    advancedToolsRepeater.model[data.index].state = checked;

                                    var cnt = 0;
                                    if (data.index === 0) {
                                        cnt = 1;
                                    }
                                    else if (data.index === 1){
                                        cnt = root.chartMaxCount - 1;
                                    }

                                    if (checked)
                                        root.chartCount += cnt;
                                    else
                                        root.chartCount -= cnt;
                                }
                            }
                        }
                    }
                }

                Column {
                    id: chartViewColumn

                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 0
                    visible: delegateroot.chartCount > 0

                    Repeater {
                        id: chartViewRepeater
                        model: modelData.classInfo

                        delegate: Rectangle {
                            id: chartRoot

                            color: "transparent"
                            radius: 5
                            height: chartHeight
                            width: parent.width

                            visible: {
                                if (index === 0) return advancedToolsRepeater.model[0].state;
                                else return advancedToolsRepeater.model[1].state;
                            }

                            ChartView {
                                anchors.fill: parent
                                legend.visible: false
                                antialiasing: true

                                LineSeries {
                                    id: series
                                    pointLabelsVisible: true
                                    pointLabelsFont.pixelSize: 15
                                    pointLabelsFormat: "@yPoint"
                                    pointLabelsClipping: false

                                    Component.onCompleted: {
                                        for (let i = 0; i < modelData.monthCountList.length; ++i) {
                                            let item = modelData.monthCountList[i];
                                            series.append(i + 1, item);
                                        }
                                    }

                                    axisY: ValuesAxis {
                                        min: 0
                                        max: maxClassCount
                                        tickType: ValuesAxis.TicksFixed
                                        tickInterval: 1
                                        labelFormat: "%d"
                                        titleText: modelData.suject + "课程节数"
                                    }

                                    axisX: CategoryAxis {
                                        min: 1
                                        max: modelData.yearMonthList.length
                                        labelsPosition: CategoryAxis.AxisLabelsPositionOnValue

                                        Component.onCompleted: {
                                            for (var i = 0; i < modelData.yearMonthList.length; i++) {
                                                append(modelData.yearMonthList[i], i + 1);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    }
}
