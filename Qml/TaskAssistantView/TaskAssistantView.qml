import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../BasicComponent"

Rectangle {
    id: root
    property var operateMode
    property var rootController
    property var controller: rootController.getTaskController()
    property string viewName

    // color: "#FFFFFF"
    radius: 5

    border {
        color: "#D6D6D6"
        width: 1
    }
    color: "#F0F4F8" // 柔和浅蓝色背景
    clip: true

    property string currentCategory: "今日"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20

        // Header with Title and Add Task Button
        RowLayout {
            Layout.fillWidth: true

            Text {
                text: "一对一助理 工作管理页面"
                font.pixelSize: 28
                font.bold: true
                color: "#333"
            }

            Item { Layout.fillWidth: true }

            Button {
                background: Rectangle {
                    color: "#0096a6"
                    radius: 6
                }
                onClicked: addTaskPopup.open()
                contentItem: Text {
                    text: "+ 添加工作"
                    font.pixelSize: 14
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                padding: 10
            }
        }

        // Category Buttons
        RowLayout {
            spacing: 10
            Layout.fillWidth: true

            Button { text: "今日任务"; onClicked: currentCategory = "今日" }
            Button { text: "本周任务"; onClicked: currentCategory = "本周" }
            Button { text: "本月任务"; onClicked: currentCategory = "本月" }
        }

        Rectangle {
            height: 2
            color: "#0096a6"
            Layout.fillWidth: true
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentWidth: availableWidth // 明确设置内容宽度跟随可用宽度

            Column {
                width: parent.width
                spacing: 20

                Repeater {
                    model: controller.taskList

                    delegate: Rectangle {
                        id: taskItem

                        property bool isDetail: false
                        property string rating: modelData.rating

                        visible: modelData.category === currentCategory
                        width: parent.width
                        radius: 12
                        color: "white"
                        border.color: "#ddd"
                        border.width: 1
                        implicitHeight: (taskItem.isDetail ? contentLayoutDetail.implicitHeight : contentLayoutSummary.implicitHeight) + 32

                        RowLayout {
                            id: contentLayoutSummary

                            anchors.margins: 16
                            anchors.fill: parent
                            spacing: 10
                            visible: !taskItem.isDetail

                            Button {
                                id: toggleArea1
                                width: 20
                                height: 24
                                onClicked: {
                                    taskItem.isDetail = !taskItem.isDetail;
                                }

                                Text {
                                    anchors.centerIn: parent
                                    text: "展开"
                                    font.pixelSize: 12
                                    color: "#444"
                                }
                            }

                            Text {
                                text: modelData.title
                                font.pixelSize: 20
                                font.bold: true
                                color: "#222"
                                wrapMode: Text.Wrap
                            }

                            RowLayout {
                                spacing: 10

                                Repeater {
                                    id: ratingItem

                                    model: 5
                                    delegate: Text {
                                        text: index < parseInt(taskItem.rating) ? "★" : "☆"
                                        font.pixelSize: 18
                                        color: "#ff9900"
                                    }
                                }
                            }

                            Item { Layout.fillWidth: true }
                            Text { text: "截止日期: " + modelData.due; color: "#555"; font.pixelSize: 13 }
                            Text {
                                text: "完成状态: " + modelData.finishStatus
                                font.pixelSize: 13
                                color: text.indexOf("未完成") !== -1 ? "red"
                                      : text.indexOf("已完成") !== -1 ? "green"
                                      : "#666"
                            }
                        }

                        ColumnLayout {
                            id: contentLayoutDetail

                            anchors.margins: 16
                            anchors.fill: parent
                            spacing: 10
                            visible: taskItem.isDetail


                            RowLayout {
                                id: titleLayout
                                spacing: 10

                                Button {
                                    id: toggleArea2
                                    width: 24
                                    height: 24
                                    onClicked: {
                                        taskItem.isDetail = !taskItem.isDetail;
                                    }

                                    Text {
                                        anchors.centerIn: parent
                                        text: "收起"
                                        font.pixelSize: 12
                                        color: "#444"
                                    }
                                }

                                Text {
                                    text: modelData.title
                                    font.pixelSize: 20
                                    font.bold: true
                                    color: "#222"
                                    wrapMode: Text.Wrap
                                }
                            }

                            Text {
                                text: modelData.description
                                font.pixelSize: 14
                                color: "#555"
                                wrapMode: Text.Wrap
                            }

                            RowLayout {
                                spacing: 10
                                Text { text: "发布日期: " + modelData.publish; font.pixelSize: 13; color: "#777" }
                                Text { text: "|"; font.pixelSize: 13; color: "#777" }
                                Text { text: "截止日期: " + modelData.due; font.pixelSize: 13; color: "#777" }
                            }

                            RowLayout {
                                spacing: 10
                                Label { text: "重要程度:" }

                                Repeater {
                                    model: 5
                                    delegate: Text {
                                        text: index < parseInt(taskItem.rating) ? "★" : "☆"
                                        font.pixelSize: 18
                                        color: "#ff9900"
                                    }
                                }
                            }

                            TextArea {
                                id: commentItem

                                placeholderText: "写入工作完成情况(至少20字)"
                                wrapMode: TextArea.Wrap
                                Layout.fillWidth: true
                                implicitHeight: Math.max(contentHeight + 12, 60)
                                font.pixelSize: 16
                                readOnly: modelData.finishStatus === "已完成"
                                text: modelData.comment
                                background: Rectangle {
                                    radius: 6
                                    border.color: "#bbb"
                                }
                            }

                            RowLayout {
                                spacing: 10
                                Layout.alignment: Qt.AlignLeft

                                Text {
                                    text: "完成状态: " + modelData.finishStatus
                                    font.pixelSize: 14
                                    color: text.indexOf("未完成") !== -1 ? "red"
                                          : text.indexOf("已完成") !== -1 ? "green"
                                          : "#666"
                                }

                                Item { Layout.fillWidth: true }

                                Button {
                                    id: finishButton

                                    visible: modelData.finishStatus === "未完成"
                                    background: Rectangle {
                                        color: "#0096a6"
                                        radius: 6
                                    }
                                    contentItem: Text {
                                        text: "完成该工作"
                                        color: "white"
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                    padding: 6
                                    onClicked: {
                                        if (commentItem.text.length < 20) {
                                            commentWarningPopup.open();
                                            return;
                                        }
                                        controller.onTaskFinished(modelData.taskId, commentItem.text);
                                        taskItem.isDetail = false;
                                    }
                                }

                            }
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: commentWarningPopup
        modal: true
        focus: true
        width: 300
        height: 150
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        background: Rectangle {
            color: "white"
            radius: 8
            border.color: "#aaa"
            border.width: 1
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 16

            Text {
                text: "请输入至少 20 个字的完成情况"
                wrapMode: Text.Wrap
                font.pixelSize: 16
                color: "#333"
            }

            Button {
                text: "确定"
                Layout.alignment: Qt.AlignHCenter
                onClicked: commentWarningPopup.close()
            }
        }
    }

    Popup {
        id: addTaskPopup
        modal: true
        focus: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 400
        height: 500
        background: Rectangle {
            color: "white"
            radius: 8
            border.color: "#aaa"
            border.width: 1
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 10

            Text { text: "添加任务"; font.pixelSize: 20; font.bold: true }

            TextField {
                id: titleField
                placeholderText: "任务标题"
                Layout.fillWidth: true
            }

            TextArea {
                id: descField
                placeholderText: "任务描述"
                Layout.fillWidth: true
                Layout.preferredHeight: 80

                background: Rectangle {
                    radius: 6
                    border.color: "#bbb"
                }
            }

            RowLayout {
                spacing: 8

                DateSelector {
                    id: publishSelector
                    Layout.fillWidth: true
                    //defaultDate: new Date()  // 默认今天
                }

                DateSelector {
                    id: dueSelector
                    Layout.fillWidth: true
                    //defaultDate: new Date()  // 默认今天
                }
            }

            RowLayout {
                spacing: 8
                Text { text: "重要程度:" }

                ComboBox {
                    id: ratingBox
                    model: ["1", "2", "3", "4", "5"]
                    currentIndex: 2
                }
            }

            ComboBox {
                id: categoryBox
                model: [
                    { name: "今日任务", value: "today" },
                    { name: "本周任务", value: "week" },
                    { name: "周期任务", value: "recurring" }
                ]
                textRole: "name"
                valueRole: "value"
                Layout.fillWidth: true
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: 10

                Button {
                    text: "取消"
                    onClicked: addTaskPopup.close()
                }

                Button {
                    text: "添加"
                    onClicked: {
                        // taskModel.append({
                        //                      "title": titleField.text,
                        //                      "desc": descField.text,
                        //                      "publish": publishField.text,
                        //                      "due": dueField.text,
                        //                      "rating": parseInt(ratingBox.currentText),
                        //                      "status": "未开始",
                        //                      "category": categoryBox.currentValue
                        //                  })
                        addTaskPopup.close()
                    }
                }
            }
        }
    }

}
