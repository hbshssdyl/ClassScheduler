import "../JSUtils/ColorUtils.js" as ColorUtils
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Window

Rectangle {
    id: root

    property var rootController
    property string viewName
    property var operateMode
    property string authMode: "none" // "login" or "register"
    property string feedbackMessage
    property color feedbackMessageColor: {
        if (authMode === "success")
            return ColorUtils.getSuccessColor();

        if (authMode === "failed")
            return ColorUtils.getErrorColor();

        return "transparent";
    }

    color: ColorUtils.getOperateAreaBackgroundColor()
    onAuthModeChanged: {
        userEmail.text = "";
        userName.text = "";
        password.text = "";
        if (authMode === "login" || authMode === "register" || authMode === "none")
            feedbackMessage = "";

    }
    onRootControllerChanged: {
        if (rootController) {
            rootController.registerOrLoginResult.connect(function(statusStr) {
                if (statusStr === "RegisterSuccess") {
                    feedbackMessage = "注册成功";
                    authMode = "success";
                } else if (statusStr === "RegisterPending") {
                    feedbackMessage = "注册申请已发出，等待管理员同意";
                    authMode = "success";
                } else if (statusStr === "UserExist") {
                    feedbackMessage = "该用户名已存在";
                    authMode = "failed";
                } else if (statusStr === "EmailExist") {
                    feedbackMessage = "该邮箱已存在";
                    authMode = "failed";
                } else if (statusStr === "EmailInvalid") {
                    feedbackMessage = "邮箱格式不合法";
                    authMode = "failed";
                } else if (statusStr === "EmptyInfo") {
                    feedbackMessage = "邮箱、用户名或密码不能为空";
                    authMode = "failed";
                } else if (statusStr === "UserOrEmailNotFound") {
                    feedbackMessage = "邮箱或用户名不存在";
                    authMode = "failed";
                } else if (statusStr === "PasswordIncorrect") {
                    feedbackMessage = "密码错误";
                    authMode = "failed";
                } else if (statusStr === "LoginFailed") {
                    feedbackMessage = "登陆失败，请重试";
                    authMode = "failed";
                } else if (statusStr === "LoginSuccess") {
                    feedbackMessage = "登录成功";
                    authMode = "success";
                } else {
                    feedbackMessage = "未知错误，请联系管理员";
                    authMode = "failed";
                }
            });
        }
    }

    ColumnLayout {
        id: layout

        spacing: 40
        anchors.fill: parent

        Item {
            id: topSpace

            Layout.fillHeight: true
        }

        Rectangle {
            id: modeSelector

            width: 300
            height: 60
            radius: 12
            Layout.alignment: Qt.AlignCenter
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
                    onClicked: authMode = "login"
                    highlighted: authMode === "login"

                    background: Rectangle {
                        color: authMode === "login" ? "#3399FF" : "transparent"
                        anchors.fill: parent
                        radius: height / 2
                        border.color: "#A4A4A4"
                        border.width: 1
                    }

                }

                RoundButton {
                    text: "注册"
                    Layout.fillWidth: true
                    onClicked: authMode = "register"
                    highlighted: authMode === "register"

                    background: Rectangle {
                        color: authMode === "register" ? "#3399FF" : "transparent"
                        anchors.fill: parent
                        radius: height / 2
                        border.color: "#A4A4A4"
                        border.width: 1
                    }

                }

            }

        }

        Rectangle {
            id: authView

            width: 320
            height: 360
            radius: 16
            color: "white"
            Layout.alignment: Qt.AlignCenter
            border.color: "#3399FF"
            border.width: 1
            visible: authMode === "login" || authMode === "register"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 24
                spacing: 16

                Text {
                    text: authMode === "login" ? "欢迎登录" : "注册新用户"
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
                    visible: authMode === "register"
                    padding: 8

                    background: Rectangle {
                        color: "#ffffff"
                        radius: 8
                        border.color: userEmail.text === "" ? "#cccccc" : "#33CC99"
                        border.width: 1
                    }

                }

                TextField {
                    id: userName

                    placeholderText: authMode === "register" ? "请输入用户名" : "请输入用户名或邮箱"
                    Layout.fillWidth: true
                    font.pixelSize: 16
                    padding: 8

                    background: Rectangle {
                        color: "#ffffff"
                        radius: 8
                        border.color: userName.text === "" ? "#cccccc" : "#33CC99"
                        border.width: 1
                    }

                }

                TextField {
                    id: password

                    placeholderText: "请输入密码"
                    echoMode: TextInput.Password
                    Layout.fillWidth: true
                    font.pixelSize: 16
                    padding: 8

                    background: Rectangle {
                        color: "#ffffff"
                        radius: 8
                        border.color: password.text === "" ? "#cccccc" : "#33CC99"
                        border.width: 1
                    }

                }

                Rectangle {
                    id: roleSelector

                    visible: authMode === "register"
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
                        model: ["员工", "高级员工", "经理", "一对一助理", "一对一经理", "一对多助理", "一对多经理", "老板"]
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

                    text: authMode === "login" ? "登录" : "注册"
                    font.pixelSize: 18
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    height: 45
                    radius: 10
                    highlighted: true
                    Keys.onEnterPressed: clicked()
                    onClicked: {
                        if (authMode === "login")
                            rootController.onTryToLogin(userName.text, password.text);
                        else if (authMode === "register")
                            rootController.onTryToRegister(userEmail.text, userName.text, password.text, roleCombo.currentText);
                    }

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

                }

            }

        }

        Rectangle {
            id: welcomeMessage

            width: 360
            height: 360
            Layout.alignment: Qt.AlignCenter
            visible: {
                if (authMode === "none")
                    return true;

                if (authMode === "success")
                    return true;

                if (authMode === "failed")
                    return true;

                return false;
            }
            color: "white"
            radius: 12
            border.color: "#3399FF"
            border.width: 1

            Image {
                id: welcomeSvg

                visible: authMode === "none"
                anchors.fill: parent
                source: "qrc:/ClassScheduler/Resource/tianming.jpg"
            }

            Image {
                id: okSvg

                visible: authMode === "success"
                width: parent.width
                height: width
                z: 1
                source: "qrc:/ClassScheduler/Resource/ok.svg"

                anchors {
                    top: parent.top
                    horizontalCenter: parent.horizontalCenter
                }

            }

            Image {
                id: notokSvg

                visible: authMode === "failed"
                width: parent.width
                height: width
                z: 1
                source: "qrc:/ClassScheduler/Resource/notok.svg"

                anchors {
                    top: parent.top
                    horizontalCenter: parent.horizontalCenter
                }

            }

            Rectangle {
                id: feedbackItem

                visible: root.feedbackMessage !== ""
                width: parent.width - 20
                height: 30
                radius: 12
                color: root.feedbackMessageColor
                border.color: "#3399FF"
                border.width: 1
                z: 2

                anchors {
                    bottom: parent.bottom
                    bottomMargin: 10
                    horizontalCenter: parent.horizontalCenter
                }

                Text {
                    id: feedbackLabel

                    anchors.centerIn: parent
                    text: root.feedbackMessage
                    color: "white"

                    font {
                        bold: false
                        pixelSize: 12
                    }

                }

            }

        }

        Item {
            id: bottomSpace

            Layout.fillHeight: true
        }

    }

}
