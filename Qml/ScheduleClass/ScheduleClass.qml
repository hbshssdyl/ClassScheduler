import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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
            id: repeater
            model: [
                { placeholder: "选择科目", options: ["语文", "数学", "英语", "物理", "化学", "生物", "道法", "跨学科", "政治", "地理", "历史"] },
                { placeholder: "选择年级", options: ["小学", "初一", "初二", "初三", "高一", "高二", "高三"] },
                { placeholder: "选择星期", options: ["星期一", "星期二", "星期三", "星期四", "星期五", "星期六", "星期日"] }
            ]

            delegate: ComboBox {
                id: comboBox
                Layout.fillWidth: true
                model: modelData.options
                font.pixelSize: 16
                implicitHeight: 30
                background: Rectangle {
                    color: "#ffffff"
                    border.color: comboBox.hovered ? "#3399FF" : "#2e7d32"
                    radius: 5
                }
                contentItem: Text {
                    text: comboBox.currentIndex === -1 ? modelData.placeholder : comboBox.currentText
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: comboBox.pressed ? "#1b5e20" : "#2e7d32"
                }
                Component.onCompleted: comboBox.currentIndex = -1
            }
        }

        Button {
            id: confirmButton
            text: "确认排课"
            font.pixelSize: 18
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            background: Rectangle {
                color: confirmButton.hovered ? "#4caff0" : "#4caf50"
                radius: 8
                border.color: "#2e7d32"
            }
            onClicked: {
                var count = repeater.count;
                var selectedValues = [];
                for (var i = 0; i < count; i++) {
                    var comboBox = repeater.itemAt(i);
                    if (comboBox.currentIndex !== -1) {
                        selectedValues.push(comboBox.currentText);
                    } else {
                        selectedValues.push("未选择");
                    }
                }
                console.log("科目:", selectedValues[0]);
                console.log("年级:", selectedValues[1]);
                console.log("星期:", selectedValues[2]);
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

