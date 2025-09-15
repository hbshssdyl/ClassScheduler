import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root

    property var operateMode
    property var rootController
    property var controller: rootController.getFeedbackController()

    color: "#f5f5f5"

    // --- 主布局 ---
    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // 标题栏
        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "#ff5722"  // 暴躁橙
            Text {
                anchors.centerIn: parent
                text: "不爽就来喷 💢"
                font {
                    pixelSize: 22
                    bold: true
                }
                color: "white"
            }
        }

        // --- 暴躁输入区 ---
        Rectangle {
            Layout.fillWidth: true
            Layout.margins: 15
            implicitHeight: inputLayout.implicitHeight + 20
            radius: 8
            color: "white"
            border.color: "#E0E0E0"

            ColumnLayout {
                id: inputLayout

                anchors.fill: parent
                anchors.margins: 12
                spacing: 12

                // 输入框
                TextArea {
                    id: complaintInput
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100
                    placeholderText: "有屁快放...(最多200字)"
                    wrapMode: Text.Wrap
                    font.pixelSize: 14
                    background: Rectangle {
                        radius: 6
                        border.color: "#ffab91"
                    }
                }

                // 匿名/实名选择
                RowLayout {
                    spacing: 15

                    RadioButton {
                        id: anonymousRadio
                        text: "怂就匿名"
                        checked: true
                        font {
                            pixelSize: 12
                            bold: true
                        }
                    }

                    RadioButton {
                        id: realNameRadio
                        text: "老子实名"
                        font {
                            pixelSize: 12
                            bold: true
                        }
                    }

                    TextField {
                        id: usernameInput
                        Layout.fillWidth: true
                        visible: !anonymousRadio.checked
                        placeholderText: "行不更名坐不改姓"
                        background: Rectangle {
                            radius: 6
                            border.color: "#ffab91"
                        }
                    }
                }

                Item { Layout.fillWidth: true }

                // 发布按钮
                Button {
                    Layout.alignment: Qt.AlignRight
                    text: "开喷！"
                    font.bold: true
                    background: Rectangle {
                        color: "#ff5722"
                        radius: 6
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                    }
                    onClicked: {
                        if(complaintInput.text.trim() === "") {
                            return;
                        }

                        var name = anonymousRadio.checked ? "匿名怂包" :
                              (usernameInput.text.trim() || "暴躁网友");
                        var dateAndTime = Qt.formatDateTime(new Date(), "yyyy年MM月dd日 - hh:mm");

                        controller.addFeedback("APP问题", name, anonymousRadio.checked, complaintInput.text, dateAndTime);

                        complaintInput.clear();
                        if(!anonymousRadio.checked) {
                            usernameInput.clear();
                        }
                    }
                }
            }
        }

        // --- 暴躁列表 ---
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 15
            clip: true

            ListView {
                model: controller.feedbackAppList
                spacing: 10
                delegate: Rectangle {
                    width: ListView.view.width
                    radius: 8
                    color: "white"
                    border {
                        color: index % 2 ? "#ffccbc" : "#ffab91"
                        width: 1
                    }
                    implicitHeight: contentColumn.implicitHeight + 20

                    ColumnLayout {
                        id: contentColumn
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 8

                        // 用户信息
                        RowLayout {
                            spacing: 10

                            Rectangle {
                                width: 30
                                height: 30
                                radius: 15
                                color: modelData.isAnonymous ? "#bdbdbd" : "#ff5722"
                                Text {
                                    anchors.centerIn: parent
                                    text: modelData.isAnonymous ? "?" : modelData.username.charAt(0)
                                    color: "white"
                                    font.bold: true
                                }
                            }

                            ColumnLayout {
                                spacing: 2
                                Text {
                                    text: modelData.username
                                    font {
                                        bold: !modelData.isAnonymous
                                        pixelSize: 14
                                    }
                                    color: modelData.isAnonymous ? "#757575" : "#d84315"
                                }
                                Text {
                                    text: modelData.dataAndTime
                                    font.pixelSize: 10
                                    color: "#9e9e9e"
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
                                        text: modelData.likes
                                        color: "#ff5722"
                                        font.pixelSize: 14
                                        Layout.alignment: Qt.AlignVCenter
                                    }
                                }
                                //onClicked: complaintsModel.setProperty(index, "likes", modelData.likes + 1)
                            }
                        }

                        // 吐槽内容
                        Text {
                            Layout.fillWidth: true
                            text: modelData.message
                            wrapMode: Text.Wrap
                            font.pixelSize: 14
                            color: "#424242"
                            Layout.topMargin: 5
                        }
                    }
                }
            }
        }
    }
}
