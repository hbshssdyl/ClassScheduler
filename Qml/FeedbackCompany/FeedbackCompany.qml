import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15

Rectangle {
    id: root

    property var operateMode
    property var rootController
    property var controller: rootController.getFeedbackController()

    color: "#f5f5f5"

    // --- 数据模型：存储吐槽内容 ---
    ListModel {
        id: complaintsModel
        ListElement {
            username: "匿名用户"
            isAnonymous: true
            content: "食堂的菜太难吃了！"
            likes: 5
            timestamp: "2小时前"
        }
        ListElement {
            username: "张三 (技术部)"
            isAnonymous: false
            content: "希望会议室预约系统能改进！"
            likes: 12
            timestamp: "昨天"
        }
    }

    // --- 主布局 ---
    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // 标题栏
        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "#4CAF50"
            Text {
                anchors.centerIn: parent
                text: "公司吐槽墙 💬"
                font.pixelSize: 20
                color: "white"
            }
        }

        // --- 吐槽输入区 ---
        Rectangle {
            id: inputArea

            Layout.fillWidth: true
            Layout.margins: 15
            implicitHeight: inputLayout.implicitHeight + 20
            radius: 10
            color: "white"
            border.color: "#E0E0E0"

            ColumnLayout {
                id: inputLayout

                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                // 输入框
                TextArea {
                    id: complaintInput
                    Layout.fillWidth: true
                    Layout.preferredHeight: 80
                    placeholderText: "老子要吐槽...（最多200字）"
                    wrapMode: Text.Wrap
                    background: Rectangle { radius: 5; border.color: "#E0E0E0" }
                }

                // 匿名/实名选择
                RowLayout {
                    RadioButton {
                        id: anonymousRadio
                        text: "匿名发布"
                        checked: true
                        font.pixelSize: 12
                        font.bold: true
                    }
                    RadioButton {
                        id: realNameRadio
                        text: "实名发布"
                        font.pixelSize: 12
                        font.bold: true
                        onCheckedChanged: {
                            if(checked) {
                                usernameInput.forceActiveFocus()
                            }
                        }
                    }
                    TextField {
                        id: usernameInput
                        Layout.fillWidth: true
                        visible: !anonymousRadio.checked
                        placeholderText: "有本事就输入你的姓名"
                        background: Rectangle {
                            radius: 5
                            border.color: "#E0E0E0"
                        }
                    }
                }

                // 发布按钮
                Button {
                    Layout.alignment: Qt.AlignRight
                    text: "发布吐槽"
                    background: Rectangle {
                        color: "#4CAF50"
                        radius: 5
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                    }
                    onClicked: {
                        var name = anonymousRadio.checked ? "匿名用户" :
                                                            (usernameInput.text.trim() === "" ? "匿名用户" : usernameInput.text);

                        complaintsModel.append({
                                                   username: name,
                                                   isAnonymous: anonymousRadio.checked,
                                                   content: complaintInput.text || "（空内容）",
                                                   likes: 0,
                                                   timestamp: "刚刚"
                                               });

                        // 清空输入
                        complaintInput.clear()
                        if(!anonymousRadio.checked) {
                            usernameInput.clear()
                        }
                    }
                }
            }
        }

        // --- 吐槽列表 ---
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 15
            clip: true

            ListView {
                model: complaintsModel
                spacing: 5
                delegate: Rectangle {
                    id: complaintsDelegate

                    width: parent.width
                    radius: 10
                    color: "white"
                    border.color: "#E0E0E0"
                    implicitHeight: contentColumn.implicitHeight + 20

                    ColumnLayout {
                        id: contentColumn
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 5

                        RowLayout {
                            spacing: 10
                            // 用户信息
                            Rectangle {
                                width: 30
                                height: 30
                                radius: 15
                                color: isAnonymous ? "#bdbdbd" : "#3A4A6B"
                                Text {
                                    anchors.centerIn: parent
                                    text: isAnonymous ? "?" : username.charAt(0)
                                    color: "white"
                                    font.bold: true
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                Text {
                                    text: username
                                    font.bold: !isAnonymous
                                    color: isAnonymous ? "#999999" : "#3A4A6B"
                                }
                                Text {
                                    text: timestamp
                                    font.pixelSize: 10
                                    color: "#999999"
                                }
                            }

                            Item { Layout.fillWidth: true }

                            // 点赞按钮
                            Button {
                                padding: 6
                                background: Rectangle {
                                    color: "transparent"
                                    radius: 10
                                }
                                contentItem: RowLayout {
                                    spacing: 5
                                    Text {
                                        text: "👍"
                                        font.pixelSize: 14
                                        Layout.alignment: Qt.AlignVCenter
                                    }
                                    Text {
                                        text: likes
                                        color: "#ff5722"
                                        font.pixelSize: 14
                                        Layout.alignment: Qt.AlignVCenter
                                    }
                                }
                                onClicked: complaintsModel.setProperty(index, "likes", likes + 1)
                            }
                        }
                        // 吐槽内容
                        Text {
                            Layout.fillWidth: true
                            text: content
                            wrapMode: Text.Wrap
                            color: "#333333"
                            Layout.preferredHeight: implicitHeight
                        }
                    }
                }
            }
        }
    }
}
