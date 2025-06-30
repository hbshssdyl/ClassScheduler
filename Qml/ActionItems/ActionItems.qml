import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Rectangle {
    id: root
    property var controller
    color: "transparent"
    clip: true

    RowLayout {
        anchors.fill: parent
        spacing: 12

        // 圆形头像 - 使用简单裁剪方法
        Rectangle {
            id: avatar
            Layout.preferredWidth: listView.height
            Layout.preferredHeight: listView.height
            radius: width / 2
            color: "transparent"
            clip: true  // 关键属性：启用裁剪

            // 头像图片
            Image {
                id: avatarImage
                anchors.fill: parent
                fillMode: Image.PreserveAspectCrop
                source: "qrc:/qt/qml/ClassScheduler/Resource/Avatar-self.jpg"

                onStatusChanged: {
                    if (status === Image.Error) {
                        console.error("头像加载失败，使用默认头像");
                        source = "qrc:/qt/qml/ClassScheduler/Resource/Avatar-self.jpg";
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

        // 操作项列表
        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: controller.actionItemsList
            orientation: ListView.Horizontal
            spacing: 12

            delegate: ActionItemDelegate {
                width: 100
                height: 50
                onButtonClicked: controller.onOperateModeSelected(mode)
            }
        }
    }
}
