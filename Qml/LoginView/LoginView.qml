import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15

Rectangle {
    id: root

    property var operateMode
    property var rootController
    color: "#f5f5f5"

    Rectangle {
        id: loginView

        width: 320
        height: 300
        radius: 16
        color: "white"
        anchors.centerIn: parent
        border.color: "#3399FF"
        border.width: 1

        ColumnLayout {
            id: loginLayout

            anchors.fill: parent
            anchors.margins: 24
            spacing: 20

            Text  {
                id: loginText

                text: "欢迎登录"
                font.pixelSize: 24
                font.bold: true
                color: "#333333"
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }

            TextField {
                id: loginUserName

                placeholderText: "请输入用户名"
                Layout.fillWidth: true
                font.pixelSize: 16
                background: Rectangle {
                    color: "#ffffff"
                    radius: 8
                    border.color: loginUserName.text === "" ? "#cccccc" : "#33CC99"
                    border.width: 1
                }
                padding: 8
            }

            TextField {
                id: loginPassword

                placeholderText: "请输入密码"
                echoMode: TextInput.Password
                Layout.fillWidth: true
                font.pixelSize: 16
                background: Rectangle {
                    color: "#ffffff"
                    radius: 8
                    border.color: loginPassword.text === "" ? "#cccccc" : "#33CC99"
                    border.width: 1
                }
                padding: 8
            }

            RoundButton {
                id: loginButton

                text: "登录"
                font.pixelSize: 18
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                height: 45
                radius: 10
                highlighted: true
                background: Rectangle{
                    color: loginButton.hovered ? "#33CCCC" : "#33CC99"
                    anchors.fill: parent
                    radius: 10
                }

                contentItem: Text {
                    id: loginButonText

                    text: "登录"
                    color: "black"
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.centerIn: parent
                }
                onClicked: {
                    rootController.onTryToLogin(loginUserName.text, loginPassword.text);
                }
            }
        }
    }

}
