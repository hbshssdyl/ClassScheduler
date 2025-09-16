import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

Rectangle {
    id: root

    property var operateMode
    property var rootController
    property var controller: rootController.getFeedbackController()

    color: "#f5f5f5"

    // 提交成功弹窗
    Popup {
        id: submitSuccessPopup
        width: 300
        height: 150
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        background: Rectangle {
            color: "white"
            radius: 10
            border.color: "#4CAF50"
            border.width: 2
        }

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 15

            Text {
                text: "💬 吐槽提交成功！"
                font {
                    pixelSize: 18
                    bold: true
                }
                color: "#4CAF50"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "系统用户已提交，等待管理员审核"
                font.pixelSize: 14
                color: "#616161"
                Layout.alignment: Qt.AlignHCenter
            }

            Button {
                text: "知道了"
                Layout.alignment: Qt.AlignHCenter
                background: Rectangle {
                    color: "#4CAF50"
                    radius: 6
                }
                contentItem: Text {
                    text: parent.text
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                }
                onClicked: submitSuccessPopup.close()
            }
        }
    }

    // 内容为空错误弹窗
    Popup {
        id: contentEmptyPopup
        width: 280
        height: 130
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        background: Rectangle {
            color: "white"
            radius: 10
            border.color: "#f44336"
            border.width: 2
        }

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 15

            Text {
                text: "❌ 提交失败！"
                font {
                    pixelSize: 18
                    bold: true
                }
                color: "#f44336"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "吐槽内容不能为空！"
                font.pixelSize: 14
                color: "#616161"
                Layout.alignment: Qt.AlignHCenter
            }

            Button {
                text: "继续吐槽"
                Layout.alignment: Qt.AlignHCenter
                background: Rectangle {
                    color: "#f44336"
                    radius: 6
                }
                contentItem: Text {
                    text: parent.text
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                }
                onClicked: contentEmptyPopup.close()
            }
        }
    }

    // 用户名为空错误弹窗
    Popup {
        id: usernameEmptyPopup
        width: 280
        height: 130
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        background: Rectangle {
            color: "white"
            radius: 10
            border.color: "#f44336"
            border.width: 2
        }

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 15

            Text {
                text: "❌ 提交失败！"
                font {
                    pixelSize: 18
                    bold: true
                }
                color: "#f44336"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "选择了实名就要留下姓名！"
                font.pixelSize: 14
                color: "#616161"
                Layout.alignment: Qt.AlignHCenter
            }

            Button {
                text: "继续吐槽"
                Layout.alignment: Qt.AlignHCenter
                background: Rectangle {
                    color: "#f44336"
                    radius: 6
                }
                contentItem: Text {
                    text: parent.text
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                }
                onClicked: usernameEmptyPopup.close()
            }
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
                font {
                    pixelSize: 22
                    bold: true
                }
                color: "white"
            }
        }

        // --- 折叠式吐槽输入区 ---
        Rectangle {
            Layout.fillWidth: true
            Layout.margins: 15
            implicitHeight: expandButton.height + (expanded ? inputLayout.implicitHeight + 40 : 20)
            radius: 8
            color: "white"
            border.color: "#E0E0E0"
            clip: true

            property bool expanded: false  // 控制展开状态

            // 展开/收起按钮
            Button {
                id: expandButton
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                text: parent.expanded ? "收起吐槽 💬" : "点击此处后开始吐槽 💬"
                font.bold: true
                background: Rectangle {
                    color: parent.expanded ? "#c8e6c9" : "#4CAF50"
                    radius: 6
                }
                contentItem: Text {
                    text: parent.text
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                }
                onClicked: {
                    parent.expanded = !parent.expanded
                }
            }

            // 输入区域（可折叠）
            ColumnLayout {
                id: inputLayout
                anchors.top: expandButton.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 12
                anchors.topMargin: parent.expanded ? 12 : 0
                spacing: 12
                visible: parent.expanded
                opacity: parent.expanded ? 1 : 0
                Behavior on opacity { NumberAnimation { duration: 200 } }

                // 输入框
                TextArea {
                    id: complaintInput
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100
                    placeholderText: "老子要吐槽...（最多200字）"
                    wrapMode: Text.Wrap
                    font.pixelSize: 14
                    background: Rectangle {
                        radius: 6
                        border.color: "#81c784"
                    }
                }

                // 匿名/实名选择
                RowLayout {
                    spacing: 15

                    RadioButton {
                        id: anonymousRadio
                        text: "匿名发布"
                        checked: true
                        font {
                            pixelSize: 12
                            bold: true
                        }
                    }

                    RadioButton {
                        id: realNameRadio
                        text: "实名发布"
                        font {
                            pixelSize: 12
                            bold: true
                        }
                    }

                    TextField {
                        id: usernameInput
                        Layout.fillWidth: true
                        visible: !anonymousRadio.checked
                        placeholderText: "有本事就输入你的姓名"
                        background: Rectangle {
                            radius: 6
                            border.color: "#81c784"
                        }
                    }
                }

                Item { Layout.fillWidth: true }

                // 发布按钮
                RowLayout {
                    Layout.alignment: Qt.AlignRight
                    spacing: 10

                    // 取消按钮
                    Button {
                        text: "算了"
                        font.bold: true
                        background: Rectangle {
                            color: "#9e9e9e"
                            radius: 6
                        }
                        contentItem: Text {
                            text: parent.text
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                        }
                        onClicked: {
                            // 收起输入区域
                            parent.parent.parent.expanded = false
                            // 清空输入
                            complaintInput.clear()
                            if(!anonymousRadio.checked) {
                                usernameInput.clear()
                            }
                        }
                    }

                    // 发布按钮
                    Button {
                        text: "发布吐槽"
                        font.bold: true
                        background: Rectangle {
                            color: "#4CAF50"
                            radius: 6
                        }
                        contentItem: Text {
                            text: parent.text
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                        }
                        onClicked: {
                            // 验证内容是否为空
                            if(complaintInput.text.trim() === "") {
                                contentEmptyPopup.open();
                                return;
                            }

                            // 验证实名用户是否输入了用户名
                            if(!anonymousRadio.checked && usernameInput.text.trim() === "") {
                                usernameEmptyPopup.open();
                                return;
                            }

                            var name = anonymousRadio.checked ? "匿名用户" :
                                                                (usernameInput.text.trim() || "公司网友");
                            var dateAndTime = Qt.formatDateTime(new Date(), "yyyy年MM月dd日 - hh:mm");

                            controller.addFeedback("公司问题", name, anonymousRadio.checked, complaintInput.text, dateAndTime);

                            // 清空输入并显示成功弹窗
                            complaintInput.clear();
                            if(!anonymousRadio.checked) {
                                usernameInput.clear();
                            }

                            // 收起输入区域
                            parent.parent.parent.expanded = false

                            // 显示提交成功弹窗
                            submitSuccessPopup.open();
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
                model: controller.feedbackCompanyList
                spacing: 10
                delegate: Rectangle {
                    width: ListView.view.width
                    radius: 8
                    color: "white"
                    visible: modelData.shouldShow
                    border {
                        color: index % 2 ? "#c8e6c9" : "#81c784"
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
                                color: modelData.isAnonymous ? "#bdbdbd" : "#3A4A6B"
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
                                    color: modelData.isAnonymous ? "#757575" : "#3A4A6B"
                                }
                                Text {
                                    text: modelData.dateAndTime
                                    font.pixelSize: 10
                                    color: "#9e9e9e"
                                }
                            }

                            Item { Layout.fillWidth: true }

                            // 状态标签
                            Rectangle {
                                visible: modelData.showAdminInfo
                                Layout.alignment: Qt.AlignRight
                                implicitWidth: statusText.width + 12
                                implicitHeight: statusText.height + 6
                                radius: 4
                                color: {
                                    switch(modelData.feedbackStatus) {
                                    case "已提交": return "#2196F3";
                                    case "已通过": return "#4CAF50";
                                    case "已拒绝": return "#F44336";
                                    default: return "#9E9E9E";
                                    }
                                }

                                Text {
                                    id: statusText
                                    anchors.centerIn: parent
                                    text: modelData.feedbackStatus
                                    color: "white"
                                    font.pixelSize: 12
                                    font.bold: true
                                }
                            }

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
                                        text: modelData.likeCount
                                        color: "#4CAF50"
                                        font.pixelSize: 14
                                        Layout.alignment: Qt.AlignVCenter
                                    }
                                }
                                onClicked: controller.likeFeedback(modelData.feedbackId);
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

                        // 真实信息区域
                        Rectangle {
                            Layout.fillWidth: true
                            Layout.topMargin: 8
                            visible: modelData.showAdminInfo
                            implicitHeight: realInfoColumn.implicitHeight + 16
                            radius: 6
                            color: "#f5f5f5"
                            border {
                                width: 1
                                color: "#e0e0e0"
                            }

                            ColumnLayout {
                                id: realInfoColumn
                                anchors.fill: parent
                                anchors.margins: 8
                                spacing: 4

                                // 真实姓名
                                RowLayout {
                                    spacing: 6
                                    Text {
                                        text: "👤"
                                        font.pixelSize: 12
                                        color: "#757575"
                                    }
                                    Text {
                                        text: modelData.realUsername
                                        font.pixelSize: 12
                                        color: "#616161"
                                        elide: Text.ElideRight
                                        Layout.fillWidth: true
                                    }
                                }

                                // 分隔线
                                Rectangle {
                                    Layout.fillWidth: true
                                    implicitHeight: 1
                                    color: "#eeeeee"
                                    Layout.topMargin: 2
                                    Layout.bottomMargin: 2
                                }

                                // 邮箱
                                RowLayout {
                                    spacing: 6
                                    Text {
                                        text: "✉️"
                                        font.pixelSize: 12
                                        color: "#757575"
                                    }
                                    Text {
                                        text: modelData.realEmail
                                        font.pixelSize: 12
                                        color: "#616161"
                                        elide: Text.ElideRight
                                        Layout.fillWidth: true
                                    }
                                }
                            }
                        }

                        // 操作按钮区域
                        RowLayout {
                            Layout.topMargin: 10
                            spacing: 8
                            visible: modelData.showAdminInfo

                            // 删除按钮
                            Button {
                                text: "删除"
                                padding: 8
                                background: Rectangle {
                                    color: "#f44336"
                                    radius: 6
                                }
                                contentItem: Text {
                                    text: parent.text
                                    color: "white"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    font.pixelSize: 12
                                    font.bold: true
                                }
                                onClicked: controller.deleteFeedback(modelData.feedbackId)
                            }

                            // 拒绝按钮（仅在状态为"已提交"时显示）
                            Button {
                                visible: modelData.feedbackStatus === "已提交"
                                text: "拒绝"
                                padding: 8
                                background: Rectangle {
                                    color: "#ff9800"
                                    radius: 6
                                }
                                contentItem: Text {
                                    text: parent.text
                                    color: "white"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    font.pixelSize: 12
                                    font.bold: true
                                }
                                onClicked: controller.rejectFeedback(modelData.feedbackId)
                            }

                            // 通过按钮（仅在状态为"已提交"时显示）
                            Button {
                                visible: modelData.feedbackStatus === "已提交"
                                text: "通过"
                                padding: 8
                                background: Rectangle {
                                    color: "#4CAF50"
                                    radius: 6
                                }
                                contentItem: Text {
                                    text: parent.text
                                    color: "white"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    font.pixelSize: 12
                                    font.bold: true
                                }
                                onClicked: controller.approveFeedback(modelData.feedbackId)
                            }

                            Item { Layout.fillWidth: true } // 占位符，使按钮右对齐
                        }
                    }
                }
            }
        }
    }
}
