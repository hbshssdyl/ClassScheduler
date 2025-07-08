import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle {
    id: root

    property var controller

    color: ColorUtils.getOperateAreaBackgroundColor()
    radius: 5
    // 圆形头像 - 使用简单裁剪方法
    Rectangle {
        id: avatar
        anchors.fill: parent
        color: "transparent"
        clip: true  // 关键属性：启用裁剪

        // 头像图片
        Image {
            id: avatarImage
            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop
            source: "qrc:/qt/qml/ClassScheduler/Resource/Banner.jpg"

            onStatusChanged: {
                if (status === Image.Error) {
                    console.error("头像加载失败，使用默认头像");
                    source = "qrc:/qt/qml/ClassScheduler/Resource/Banner.jpg";
                }
            }
        }

        // 圆形边框
        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: "transparent"
            border.color: "#888"
            border.width: 1
        }

        // 点击区域
        MouseArea {
            anchors.fill: parent
            onClicked: settingsMenu.open()
        }
    }

    // 设置菜单
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



