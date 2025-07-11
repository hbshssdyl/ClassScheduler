import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle {
    id: root

    property var controller
    property string username: "User" // You can bind this to your actual username

    color: ColorUtils.getOperateAreaBackgroundColor()

    Rectangle {
        id: avatar
        anchors.fill: parent
        color: "transparent"
        clip: true

        Image {
            id: avatarImage
            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop
            source: "qrc:/qt/qml/ClassScheduler/Resource/Banner-mountain.jpg"
        }

        // Blurred background for the text
        Rectangle {
            id: textBackground
            width: welcomeText.width + 20
            height: welcomeText.height + 20
            anchors {
                left: parent.left
                leftMargin: 5
                verticalCenter: parent.verticalCenter
            }
            radius: 8

            // Create a blurred background using the image
            ShaderEffectSource {
                id: blurSource
                sourceItem: avatarImage
                sourceRect: Qt.rect(textBackground.x, textBackground.y, textBackground.width, textBackground.height)
            }

            FastBlur {
                anchors.fill: parent
                source: blurSource
                radius: 32
                transparentBorder: true
            }

            // Semi-transparent overlay
            Rectangle {
                anchors.fill: parent
                color: "#40000000"
                radius: parent.radius
            }

            // Welcome text
            Text {
                id: welcomeText
                anchors.centerIn: parent
                text: "欢迎 " + controller.name
                color: "white"
                font {
                    pixelSize: 14
                    bold: true
                }
                renderType: Text.NativeRendering
            }
            MouseArea {
                anchors.fill: parent
                onClicked: settingsMenu.open()
            }
        }


    }

    Menu {
        id: settingsMenu
        width: 200
        y: avatar.height

        MenuItem {
            text: "个人设置"
            onTriggered: console.log("个人设置点击")
        }
        MenuItem {
            text: "账户管理"
            onTriggered: console.log("账户管理点击")
        }
        MenuSeparator {}
        MenuItem {
            text: "退出"
            onTriggered: console.log("退出点击")
        }
    }
}
