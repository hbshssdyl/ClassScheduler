﻿import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    property var operateMode
    property var rootController
    property string viewName
    property var controller: rootController.getScheduleClassController()
    // property int startTimeH: 8
    // property int startTimeM: 0
    // property int endTimeH: 23
    // property int endTimeM: 60

    function generateTimeModel(isStartTime) {
        var timeList = [];
        var startTimeH = 8;
        var startTimeM = 0;
        var endTimeH = 23;
        var endTimeM = 60;
        // if(isStartTime) {
        //     endTimeH = root.endTimeH;
        //     endTimeM = root.endTimeM;
        // }
        // else{
        //     startTimeH = root.startTimeH;
        //     startTimeM = root.startTimeM;
        // }

        for (var h = startTimeH; h < endTimeH; h++) {
            for (var m = startTimeM; m < endTimeM; m += 5) {
                var hour = h < 10 ? "0" + h : h;
                var minute = m < 10 ? "0" + m : m;
                timeList.push(hour + ":" + minute);
            }
        }
        if(isStartTime) {
            timeList.push("23:00");
        }
        return timeList;
    }

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

        Rectangle {
            id: schedulerRoot

            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: parent.width / 2 - 10
            Layout.fillHeight: true
            Layout.margins: 5
            color: "#e8f5e9" // 柔和的绿色背景
            radius: 5
            clip: true

            border {
                color: "#D6D6D6"
                width: 1
            }

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 20
                width: parent.width * 0.8

                Text {
                    id: titleText
                    text: "一对一排课"
                    font.pixelSize: 28
                    font.bold: true
                    color: "#2e7d32" // 深绿色字体
                    horizontalAlignment: Text.AlignHCenter
                    Layout.alignment: Qt.AlignHCenter
                }

                Repeater {
                    id: repeater
                    model: [
                        { placeholder: "选择科目", options: ["语文", "数学", "英语", "物理", "化学", "生物", "道法", "跨学科", "政治", "地理", "历史"] },
                        { placeholder: "选择年级", options: ["小学", "预备", "初一", "初二", "初三", "高一", "高二", "高三"] },
                        { placeholder: "选择星期", options: ["星期一", "星期二", "星期三", "星期四", "星期五", "星期六", "星期日"] }
                    ]

                    delegate: ComboBox {
                        id: comboBox

                        property bool isValid: true

                        Layout.fillWidth: true
                        model: modelData.options
                        font.pixelSize: 16
                        implicitHeight: 30

                        background: Rectangle {
                            color: isValid ? "#ffffff" : "red"
                            border.color: comboBox.hovered ? "#3399FF" : "#2e7d32"
                            border.width: isValid ? 1 : 2
                            radius: 5
                        }

                        contentItem: Text {
                            text: comboBox.currentIndex === -1 ? modelData.placeholder : comboBox.currentText
                            font.pixelSize: 16
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            color: comboBox.pressed ? "#1b5e20" : "#2e7d32"
                        }

                        onCurrentIndexChanged:{
                            comboBox.isValid = true;
                        }

                        Component.onCompleted: comboBox.currentIndex = -1
                    }
                }

                RowLayout {
                    id: timeLayout

                    spacing: 5

                    Repeater {
                        id: timeRepeater
                        model: [
                            { placeholder: "开始时间", options: generateTimeModel() },
                            { placeholder: "结束时间", options: generateTimeModel() }
                        ]

                        delegate: ComboBox {
                            id: timeComboBox

                            property bool isValid: true
                            property bool isStartTime: index == 1

                            Layout.fillWidth: true
                            model: modelData.options
                            font.pixelSize: 16
                            implicitHeight: 30
                            clip: true

                            background: Rectangle {
                                color: isValid ? "#ffffff" : "red"
                                border.color: timeComboBox.hovered ? "#3399FF" : "#2e7d32"
                                border.width: isValid ? 1 : 2
                                radius: 5
                            }

                            contentItem: Text {
                                text: timeComboBox.currentIndex === -1 ? modelData.placeholder : timeComboBox.currentText
                                font.pixelSize: 16
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                leftPadding: 5
                                color: timeComboBox.pressed ? "#1b5e20" : "#2e7d32"
                            }

                            onCurrentIndexChanged:{
                                timeComboBox.isValid = true;
                                // var tmp = timeComboBox.currentText.split(":");
                                // if (index == 1)
                                // {
                                //     root.startTimeH = parseInt(tmp[0], 10);
                                //     root.startTimeM = parseInt(tmp[1], 10);
                                // }
                                // else
                                // {
                                //     root.endTimeH =  parseInt(tmp[0], 10);
                                //     root.endTimeM =  parseInt(tmp[1], 10);
                                // }
                            }

                            Component.onCompleted: timeComboBox.currentIndex = -1

                        }
                    }


                }
                Button {
                    id: confirmButton
                    text: "确认排课"
                    font.pixelSize: 16
                    font.bold: true
                    clip: true
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    background: Rectangle {
                        color: confirmButton.hovered ? "#4caff0" : "#4caf50"
                        radius: 8
                        border.color: "#2e7d32"
                    }
                    onClicked: {
                        var infosList = [];
                        let isValidList = true;

                        var count = repeater.count;
                        var noDataIndexs = [];
                        for (var i = 0; i < count; i++) {
                            var comboBox = repeater.itemAt(i);
                            if (comboBox.currentIndex !== -1) {
                                infosList.push(comboBox.currentText);
                            } else {
                                comboBox.isValid = false;
                                isValidList = false;
                            }
                        }

                        var timeCount = timeRepeater.count;
                        for (var j = 0; j < timeCount; j++) {
                            var timeComboBox = timeRepeater.itemAt(j);
                            if (timeComboBox.currentIndex !== -1) {
                                infosList.push(timeComboBox.currentText);
                            } else {
                                timeComboBox.isValid = false;
                                isValidList = false;
                            }
                        }

                        if(isValidList)
                        {
                            controller.onRequiredInfosReceived(infosList);
                        }
                    }
                    contentItem: Text {
                        text: confirmButton.text
                        color: "#ffffff"
                        font.pixelSize: 18
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
        Rectangle {
            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: parent.width / 2 - 10
            Layout.fillHeight: true
            Layout.margins: 5
            color: "#e8f5e9"
            radius: 5

            border {
                color: "#D6D6D6"
                width: 1
            }

            ListView {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 10
                clip: true
                model: controller.scheduleClassResultsList

                delegate: Item {
                    width: ListView.view.width
                    height: contentLayout.implicitHeight + 20

                    Rectangle {
                        anchors.fill: parent
                        radius: 10
                        color: "#ffffff" // White card background
                        border.color: "#cccccc"
                        border.width: 1

                        ColumnLayout {
                            id: contentLayout

                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 5

                            Repeater {
                                model: modelData

                                TextEdit {

                                    selectByMouse: true
                                    readOnly: true
                                    text: "<b>" + modelData["label"] + "</b>" + modelData["value"]
                                    font.pixelSize: 14
                                    font.bold: false
                                    textFormat: Text.RichText
                                    color: "#555555"
                                    Layout.fillWidth: true
                                    wrapMode: TextEdit.WordWrap
                                }
                            }
                        }
                    }
                }
            }
        }

    }
}
