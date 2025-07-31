import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    property var operateMode
    property var rootController
    property string viewName

    // color: "#FFFFFF"
    radius: 5

    border {
        color: "#D6D6D6"
        width: 1
    }
    color: "#F0F4F8" // 柔和浅蓝色背景

    property string currentCategory: "today"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20

        // Header with Title and Add Task Button
        RowLayout {
            Layout.fillWidth: true

            Text {
                text: "Task Monitoring"
                font.pixelSize: 28
                font.bold: true
                color: "#333"
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "+ Add Task"
                background: Rectangle {
                    color: "#0096a6"
                    radius: 6
                }
                onClicked: addTaskPopup.open()
                contentItem: Text {
                    text: "+ Add Task"
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

            Button { text: "Today"; onClicked: currentCategory = "today" }
            Button { text: "This Week"; onClicked: currentCategory = "week" }
            Button { text: "Recurring"; onClicked: currentCategory = "recurring" }
        }

        Rectangle {
            height: 2
            color: "#0096a6"
            Layout.fillWidth: true
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Column {
                width: parent.width
                spacing: 20

                Repeater {
                    model: ListModel {
                        ListElement { title: "Today’s Task"; desc: "Description..."; publish: "Apr 24, 2024"; due: "Apr 24, 2024"; rating: "3"; status: "In Progress"; category: "today" }
                        ListElement { title: "Weekly Review"; desc: "Description..."; publish: "Apr 20, 2024"; due: "Apr 26, 2024"; rating: "4"; status: "Not Started"; category: "week" }
                        ListElement { title: "Monthly Report"; desc: "Description..."; publish: "Apr 01, 2024"; due: "Apr 30, 2024"; rating: "5"; status: "Completed"; category: "recurring" }
                    }

                    delegate: Rectangle {
                        visible: model.category === currentCategory
                        width: parent.width
                        radius: 12
                        color: "white"
                        border.color: "#ddd"
                        border.width: 1
                        implicitHeight: contentLayout.implicitHeight + 32

                        ColumnLayout {
                            id: contentLayout

                            anchors.margins: 16
                            anchors.fill: parent
                            spacing: 10

                            Text {
                                text: model.title
                                font.pixelSize: 20
                                font.bold: true
                                color: "#222"
                                wrapMode: Text.Wrap
                            }

                            Text {
                                text: model.desc
                                font.pixelSize: 14
                                color: "#555"
                                wrapMode: Text.Wrap
                            }

                            RowLayout {
                                spacing: 10
                                Text { text: "Published: " + model.publish; font.pixelSize: 13; color: "#777" }
                                Text { text: "|"; font.pixelSize: 13; color: "#777" }
                                Text { text: "Due: " + model.due; font.pixelSize: 13; color: "#777" }
                            }

                            RowLayout {
                                spacing: 10
                                Label { text: "重要程度:" }

                                Repeater {
                                    model: 5
                                    delegate: Text {
                                        text: index < parseInt(model.rating) ? "★" : "☆"
                                        font.pixelSize: 18
                                        color: "#ff9900"
                                    }
                                }
                            }

                            TextArea {
                                placeholderText: "Write comment"
                                wrapMode: TextArea.Wrap
                                Layout.fillWidth: true
                                implicitHeight: Math.max(contentHeight + 12, 60)
                                font.pixelSize: 14
                                background: Rectangle {
                                    radius: 6
                                    border.color: "#bbb"
                                }
                            }

                            RowLayout {
                                spacing: 10
                                Layout.alignment: Qt.AlignLeft

                                Text { text: "Rating:"; font.pixelSize: 14; color: "#333" }
                                ComboBox {
                                    model: ["1", "2", "3", "4", "5"]
                                    currentIndex: parseInt(model.rating) - 1
                                    width: 100
                                }

                                Text { text: "Status: " + model.status; font.pixelSize: 14; color: "#666" }

                                Item { Layout.fillWidth: true }

                                Button {
                                    text: "Submit"
                                    background: Rectangle {
                                        color: "#0096a6"
                                        radius: 6
                                    }
                                    contentItem: Text {
                                        text: "Submit"
                                        color: "white"
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                    padding: 6
                                }

                            }
                        }
                    }
                }
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
            }

            RowLayout {
                spacing: 8
                TextField {
                    id: publishField
                    placeholderText: "发布日期 (YYYY-MM-DD)"
                    Layout.fillWidth: true
                }
                TextField {
                    id: dueField
                    placeholderText: "截止日期"
                    Layout.fillWidth: true
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
                        taskModel.append({
                            "title": titleField.text,
                            "desc": descField.text,
                            "publish": publishField.text,
                            "due": dueField.text,
                            "rating": parseInt(ratingBox.currentText),
                            "status": "未开始",
                            "category": categoryBox.currentValue
                        })
                        addTaskPopup.close()
                    }
                }
            }
        }
    }

}
