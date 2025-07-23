import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15

Rectangle {
    id: root
    width: 400
    height: 400
    color: "#f5f5f5"

    property var operateMode: "none" // "login" or "register"
    property var rootController
    property string feedbackMessage: ""

    onOperateModeChanged: {
        userEmail.text = "";
        userName.text = "";
        password.text = "";
    }

    Rectangle {
        id: modeSelector
        width: 300
        height: 60
        radius: 12
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 40
        color: "white"
        border.color: "#3399FF"
        border.width: 1

        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            RoundButton {
                text: "登录"
                Layout.fillWidth: true
                onClicked: operateMode = "login"
                highlighted: operateMode === "login"
            }
            RoundButton {
                text: "注册"
                Layout.fillWidth: true
                onClicked: operateMode = "register"
                highlighted: operateMode === "register"
            }
        }
    }

    Rectangle {
        id: authView
        width: 320
        height: 360
        radius: 16
        color: "white"
        anchors.top: modeSelector.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        border.color: "#3399FF"
        border.width: 1
        visible: operateMode !== "none"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 16

            Text {
                text: operateMode === "login" ? "欢迎登录" : "注册新用户"
                font.pixelSize: 24
                font.bold: true
                color: "#333333"
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }

            TextField {
                id: userEmail
                placeholderText: "请输入邮箱"
                Layout.fillWidth: true
                font.pixelSize: 16
                visible: operateMode === "register"
                background: Rectangle {
                    color: "#ffffff"
                    radius: 8
                    border.color: userEmail.text === "" ? "#cccccc" : "#33CC99"
                    border.width: 1
                }
                padding: 8
            }

            TextField {
                id: userName
                placeholderText: operateMode === "register" ? "请输入用户名" : "请输入用户名或邮箱"
                Layout.fillWidth: true
                font.pixelSize: 16
                background: Rectangle {
                    color: "#ffffff"
                    radius: 8
                    border.color: userName.text === "" ? "#cccccc" : "#33CC99"
                    border.width: 1
                }
                padding: 8
            }

            TextField {
                id: password
                placeholderText: "请输入密码"
                echoMode: TextInput.Password
                Layout.fillWidth: true
                font.pixelSize: 16
                background: Rectangle {
                    color: "#ffffff"
                    radius: 8
                    border.color: password.text === "" ? "#cccccc" : "#33CC99"
                    border.width: 1
                }
                padding: 8
            }

            Rectangle {
                id: roleSelector
                visible: operateMode === "register"
                Layout.fillWidth: true
                height: 40
                radius: 8
                border.color: "#33CC99"
                border.width: 1
                clip: true
                color: "#ffffff"

                ComboBox {
                    id: roleCombo
                    anchors.fill: parent
                    model: ["普通员工", "高级员工"]
                    font.pixelSize: 16

                    // 自定义 ComboBox 的背景
                    background: Rectangle {
                        color: "transparent" // 背景透明，显示 roleSelector 的样式
                        radius: roleSelector.radius // 继承 roleSelector 的圆角
                        border.color: roleSelector.border.color // 继承 roleSelector 的边框颜色
                        border.width: roleSelector.border.width // 继承 roleSelector 的边框宽度
                    }

                    // 自定义下拉列表的样式（可选，确保下拉框也符合样式）
                    popup: Popup {
                        y: roleCombo.height // 下拉框位置
                        width: roleCombo.width
                        implicitHeight: contentItem.implicitHeight
                        padding: 1

                        contentItem: ListView {
                            clip: true
                            implicitHeight: contentHeight
                            model: roleCombo.popup.visible ? roleCombo.delegateModel : null
                            currentIndex: roleCombo.highlightedIndex
                        }

                        background: Rectangle {
                            radius: roleSelector.radius // 下拉框也使用相同的圆角
                            border.color: roleSelector.border.color
                            border.width: roleSelector.border.width
                        }
                    }

                    delegate: ItemDelegate {
                        text: modelData
                        width: roleCombo.width
                        height: roleCombo.height
                    }
                }
            }

            RoundButton {
                id: actionButton
                text: operateMode === "login" ? "登录" : "注册"
                font.pixelSize: 18
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                height: 45
                radius: 10
                highlighted: true
                background: Rectangle {
                    color: actionButton.hovered ? "#33CCCC" : "#33CC99"
                    anchors.fill: parent
                    radius: 10
                }

                contentItem: Text {
                    text: actionButton.text
                    color: "black"
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.centerIn: parent
                }

                onClicked: {
                    if (operateMode === "login") {
                        rootController.onTryToLogin(userName.text, password.text)
                    } else if (operateMode === "register") {
                        rootController.onTryToRegister(userName.text, password.text, roleCombo.currentText)
                    }
                }
            }

            Text {
                id: feedbackLabel
                text: feedbackMessage
                color: "#FF3333"
                font.pixelSize: 14
                visible: feedbackMessage.length > 0
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }

    // 展示欢迎信息占位内容
    Rectangle {
        id: welcomeMessage
        width: parent.width
        height: 160
        anchors.top: modeSelector.bottom
        anchors.topMargin: 20
        visible: operateMode === "none"
        color: "transparent"

        Column {
            anchors.centerIn: parent
            spacing: 10

            Text {
                text: "欢迎使用 ClassScheduler!"
                font.pixelSize: 20
                color: "#444"
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: "请选择上方操作进行登录或注册。"
                font.pixelSize: 14
                color: "#777"
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
