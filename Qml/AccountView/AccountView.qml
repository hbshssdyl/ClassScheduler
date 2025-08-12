import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: root

    property var operateMode
    property var rootController
    property var controller: rootController.getAccountViewController()
    property string viewName

    background: Rectangle { color: "#e8f5e9" }

    // 添加账号弹窗
    Popup {
        id: addUserPopup
        width: 400
        height: 450
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        anchors.centerIn: parent
        background: Rectangle {
            color: "white"
            radius: 10
            border.color: "#E0E0E0"
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15

            Label {
                text: "添加新账号"
                font.bold: true
                font.pixelSize: 18
                color: "#3A4A6B"
                Layout.alignment: Qt.AlignHCenter
            }

            GridLayout {
                columns: 2
                columnSpacing: 15
                rowSpacing: 10

                Label { text: "用户名:"; color: "#333333" }
                TextField {
                    id: newUsername
                    Layout.fillWidth: true
                    background: Rectangle {
                        radius: 5;
                        border.color: "#E0E0E0"
                    }
                }

                Label { text: "密码:"; color: "#333333" }
                TextField {
                    id: newPassword
                    Layout.fillWidth: true
                    echoMode: TextInput.Password
                    background: Rectangle {
                        radius: 5;
                        border.color: "#E0E0E0"
                    }
                }

                Label { text: "确认密码:"; color: "#333333" }
                TextField {
                    id: confirmPassword
                    Layout.fillWidth: true
                    echoMode: TextInput.Password
                    background: Rectangle {
                        radius: 5;
                        border.color: "#E0E0E0"
                    }
                }

                Label { text: "邮箱:"; color: "#333333" }
                TextField {
                    id: newEmail
                    Layout.fillWidth: true
                    background: Rectangle {
                        radius: 5;
                        border.color: "#E0E0E0"
                    }
                }

                Label { text: "权限:"; color: "#333333" }
                ComboBox {
                    id: newRole
                    Layout.fillWidth: true
                    height: newEmail.height
                    model: ["普通用户", "管理员", "超级管理员"]
                    background: Rectangle {
                        radius: 5;
                        border.color: "#E0E0E0"
                        implicitHeight: newEmail.height  // 确保默认高度
                    }
                }
            }

            Label {
                id: passwordError
                color: "#FF6B6B"
                visible: text !== ""
                Layout.alignment: Qt.AlignHCenter
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: 10

                Button {
                    text: "取消"
                    background: Rectangle {
                        radius: 5
                        color: "transparent"
                        border.color: "#E0E0E0"
                    }
                    onClicked: addUserPopup.close()
                }

                Button {
                    text: "确认添加"
                    background: Rectangle {
                        radius: 5
                        color: "#4CAF50"
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                    }
                    onClicked: {
                        if(newPassword.text !== confirmPassword.text) {
                            passwordError.text = "两次输入的密码不一致"
                            return
                        }

                        console.debug("添加账号");

                        // 清空输入
                        newUsername.text = ""
                        newPassword.text = ""
                        confirmPassword.text = ""
                        newEmail.text = ""
                        passwordError.text = ""

                        addUserPopup.close()
                    }
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // 顶部标题栏
        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "#4CAF50"
            Text {
                anchors.centerIn: parent
                text: "账号管理系统"
                font.pixelSize: 20
                color: "white"
            }

            // 添加账号按钮
            Button {
                anchors {
                    right: parent.right
                    rightMargin: 15
                    verticalCenter: parent.verticalCenter
                }
                text: "+ 添加账号"
                background: Rectangle {
                    radius: 5
                    color: "#3A4A6B"
                }
                contentItem: Text {
                    text: parent.text
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                }
                onClicked: addUserPopup.open()
            }
        }

        // 标签页 - 美化版
        TabBar {
            id: tabBar
            Layout.fillWidth: true
            Layout.leftMargin: 15
            Layout.rightMargin: 15
            background: Rectangle { color: "transparent" }

            TabButton {
                text: "新用户审核 (" + controller.pendingAccountList.length + ")"
                Layout.fillWidth: true  // 改为使用布局属性

                background: Rectangle {
                    color: tabBar.currentIndex === 0 ? "#4CAF50" : "#E0E0E0"
                    radius: 5
                }

                contentItem: Text {
                    text: parent.text
                    color: tabBar.currentIndex === 0 ? "white" : "#333333"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            TabButton {
                text: "已注册用户 (" + controller.finishedAccountList.length + ")"
                Layout.fillWidth: true  // 改为使用布局属性

                background: Rectangle {
                    color: tabBar.currentIndex === 1 ? "#4CAF50" : "#E0E0E0"
                    radius: 5
                }

                contentItem: Text {
                    text: parent.text
                    color: tabBar.currentIndex === 1 ? "white" : "#333333"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: 15
            Layout.rightMargin: 15
            currentIndex: tabBar.currentIndex

            // 新用户审核页面
            ScrollView {
                clip: true
                background: Rectangle { color: "transparent" }

                ListView {
                    id: pendingUsersList
                    model: controller.pendingAccountList
                    spacing: 10
                    boundsBehavior: Flickable.StopAtBounds

                    delegate: Rectangle {
                        width: pendingUsersList.width
                        implicitHeight: contentColumn1.implicitHeight + 20
                        radius: 10
                        color: "white"
                        border.color: "#E0E0E0"

                        ColumnLayout {
                            id: contentColumn1
                            anchors.fill: parent
                            anchors.margins: 15
                            spacing: 5

                            GridLayout {
                                id: pendingAccoundLayout

                                property bool showPassword: false
                                columns: 2
                                columnSpacing: 15
                                rowSpacing: 10

                                Label { text: "用户名:"; color: "#999999" }
                                Label { text: modelData.username; color: "#333333"; font.bold: true }

                                Label { text: "密码:"; color: "#999999" }
                                RowLayout {
                                    spacing: 5
                                    Layout.fillWidth: true

                                    Text {
                                        id: passwordLabel
                                        text: pendingAccoundLayout.showPassword ? modelData.password : "*".repeat(modelData.password.length)
                                        color: "#333333"
                                        font.bold: true
                                        Layout.alignment: Qt.AlignVCenter
                                    }

                                    ToolButton {
                                        text: pendingAccoundLayout.showPassword ? "🙈" : "👁"
                                        font.pixelSize: 14
                                        onClicked: pendingAccoundLayout.showPassword = !pendingAccoundLayout.showPassword
                                    }
                                }

                                Label { text: "邮箱:"; color: "#999999" }
                                Label { text: modelData.email; color: "#333333" }

                                Label { text: "申请权限:"; color: "#999999" }
                                Label {
                                    text: modelData.role
                                    color: modelData.role === "超级管理员" ? "#FF6B6B" :
                                                                        ((modelData.role.includes("员工") || modelData.role.includes("助理")) ? "#3A4A6B" : "#4CAF50")
                                    font.bold: true
                                }

                                Label { text: "账号状态:"; color: "#999999" }
                                Label {
                                    text: modelData.account_status
                                    color: modelData.account_status === "已通过" ? "#4CAF50" :
                                                                                modelData.account_status === "已注册" ? "#3A4A6B" : "#FF6B6B"
                                    font.bold: true
                                }
                            }

                            RowLayout {
                                Layout.alignment: Qt.AlignRight
                                spacing: 10

                                Button {
                                    text: "拒绝"
                                    background: Rectangle {
                                        radius: 5
                                        color: "transparent"
                                        border.color: "#E0E0E0"
                                    }
                                    onClicked: {console.debug("拒绝")}
                                }

                                Button {
                                    text: "同意"
                                    background: Rectangle {
                                        radius: 5
                                        color: "#4CAF50"
                                    }
                                    contentItem: Text {
                                        text: parent.text
                                        color: "white"
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                    onClicked: {
                                        console.debug("同意")
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // 已注册用户页面
            ScrollView {
                clip: true
                background: Rectangle { color: "transparent" }

                ListView {
                    id: registeredUsersList
                    model: controller.finishedAccountList
                    spacing: 10
                    boundsBehavior: Flickable.StopAtBounds

                    delegate: Rectangle {
                        width: registeredUsersList.width
                        implicitHeight: contentColumn2.implicitHeight + 20
                        radius: 10
                        color: "white"
                        border.color: "#E0E0E0"

                        ColumnLayout {
                            id: contentColumn2
                            anchors.fill: parent
                            anchors.margins: 15
                            spacing: 5

                            GridLayout {
                                id: finishedAccoundLayout

                                columns: 2
                                columnSpacing: 15
                                rowSpacing: 10

                                property bool showPassword: false

                                Label { text: "用户名:"; color: "#999999" }
                                Label { text: modelData.username; color: "#333333"; font.bold: true }

                                Label { text: "密码:"; color: "#999999" }
                                RowLayout {
                                    spacing: 5
                                    Layout.fillWidth: true

                                    Text {
                                        text: finishedAccoundLayout.showPassword ? modelData.password : "*".repeat(modelData.password.length)
                                        color: "#333333"
                                        font.bold: true
                                        Layout.alignment: Qt.AlignVCenter
                                    }

                                    ToolButton {
                                        text: finishedAccoundLayout.showPassword ? "\u{1F513}" : "\u{1F512}"
                                        font.pixelSize: 14
                                        onClicked: finishedAccoundLayout.showPassword = !finishedAccoundLayout.showPassword
                                    }
                                }

                                Label { text: "邮箱:"; color: "#999999" }
                                Label { text: modelData.email; color: "#333333" }

                                Label { text: "权限:"; color: "#999999" }
                                Label {
                                    text: modelData.role
                                    color: modelData.role === "超级管理员" ? "#FF6B6B" :
                                                                        ((modelData.role.includes("员工") || modelData.role.includes("助理")) ? "#3A4A6B" : "#4CAF50")
                                    font.bold: true
                                }

                                Label { text: "账号状态:"; color: "#999999" }
                                Label {
                                    text: modelData.account_status
                                    color: modelData.account_status === "已通过" ? "#4CAF50" :
                                                                                modelData.account_status === "已注册" ? "#3A4A6B" : "#FF6B6B"
                                    font.bold: true
                                }
                            }

                            RowLayout {
                                Layout.alignment: Qt.AlignRight

                                Button {
                                    text: "删除账号"
                                    background: Rectangle {
                                        radius: 5
                                        color: "transparent"
                                        border.color: "#FF6B6B"
                                    }
                                    contentItem: Text {
                                        text: parent.text
                                        color: "#FF6B6B"
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                    onClicked: {console.debug("删除账号")}
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
