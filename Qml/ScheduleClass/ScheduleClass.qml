import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root

    property var rootController

    color: "#e8f5e9" // 柔和的绿色背景

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20
        width: parent.width * 0.8

        Text {
            id: titleText
            text: "学生排课系统"
            font.pixelSize: 28
            font.bold: true
            color: "#2e7d32" // 深绿色字体
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
        }

        Repeater {
            model: [
                { placeholder: "选择科目", options: ["语文", "数学", "英语", "物理", "化学", "生物", "道法"] },
                { placeholder: "选择年级", options: ["小学", "初一", "初二", "初三", "高一", "高二", "高三"] },
                { placeholder: "选择星期", options: ["星期一", "星期二", "星期三", "星期四", "星期五", "星期六", "星期日"] }
            ]
            delegate: ComboBox {
                id: comboBox

                Layout.fillWidth: true
                model: modelData.options
                font.pixelSize: 16
                currentIndex: -1
                //placeholderText: modelData.placeholder
                background: Rectangle {
                    color: "#ffffff"
                    border.color: "#2e7d32"
                    radius: 5
                }
                contentItem: Text {
                    text: comboBox.currentText
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: comboBox.pressed ? "#1b5e20" : "#2e7d32"
                }
            }
        }
        // ComboBox {
        //     id: subjectComboBox
        //     Layout.fillWidth: true
        //     model: ["语文", "数学", "英语", "物理", "化学", "生物", "道法"]
        //     font.pixelSize: 16
        //     background: Rectangle {
        //         color: "#ffffff"
        //         border.color: "#2e7d32"
        //         radius: 5
        //     }
        //     contentItem: Text {
        //         text: subjectComboBox.currentText
        //         font.pixelSize: 16
        //         horizontalAlignment: Text.AlignHCenter
        //         verticalAlignment: Text.AlignVCenter
        //         color: subjectComboBox.pressed ? "#1b5e20" : "#2e7d32"
        //     }
        // }

        // ComboBox {
        //     id: gradeComboBox
        //     Layout.fillWidth: true
        //     model: ["小学", "初一", "初二", "初三", "高一", "高二", "高三"]
        //     font.pixelSize: 16
        //     background: Rectangle {
        //         color: "#ffffff"
        //         border.color: "#2e7d32"
        //         radius: 5
        //     }
        //     contentItem: Text {
        //         text: gradeComboBox.currentText
        //         font.pixelSize: 16
        //         horizontalAlignment: Text.AlignHCenter
        //         verticalAlignment: Text.AlignVCenter
        //         color: gradeComboBox.pressed ? "#1b5e20" : "#2e7d32"
        //     }
        // }



        Button {
            id: confirmButton
            text: "确认排课"
            font.pixelSize: 18
            font.bold: true
            hoverEnabled: false
            Layout.alignment: Qt.AlignHCenter
            background: Rectangle {
                color: mouseArea.containsMouse ? "#4caff0" : "#4caf50" // 按钮绿色背景
                radius: 8
                border.color: "#2e7d32"
            }

            MouseArea {
                id: mouseArea

                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    if (subjectComboBox.currentIndex === -1 || gradeComboBox.currentIndex === -1 || dayComboBox.currentIndex === -1) {
                        console.log("请完整填写所有信息")
                    } else {
                        console.log("排课信息：", subjectComboBox.currentText, gradeComboBox.currentText, dayComboBox.currentText)
                    }
                }
            }

            contentItem: Text {
                text: confirmButton.text
                color: "#ffffff" // 按钮文字白色
                font.pixelSize: 18
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }


        }
    }
}

