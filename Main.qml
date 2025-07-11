import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import Controller 1.0
import ClassScheduler 1.0

Window {
    width: 1000
    height: 618
    visible: true
    title: qsTr("天明书院信息管理系统")

    Controller{
        id: controller

        objectName: "controller"
    }

    Component.onCompleted: {
        console.debug("onCompleted");
    }

    ColumnLayout {
        id: columnLayout

        anchors.fill: parent

        UserInfo {
            id: userInfo

            visible: controller.showUserInfo
            controller: controller
            Layout.alignment: Qt.AlignTop
            Layout.preferredHeight: 50
            Layout.fillWidth: true
            Layout.leftMargin: 12
            Layout.rightMargin: 12
        }

        RowLayout {
            id: rowLayout

            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            ActionItems {
                id: actionItems

                controller: controller
                visible: controller.showActions
                Layout.alignment: Qt.AlignTop
                Layout.preferredWidth: 100
                Layout.fillHeight: true
                Layout.leftMargin: 12
                Layout.bottomMargin: 12
            }

            OperateArea{
                id: operateArea

                controller: controller
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 12
                Layout.bottomMargin: 12
                Layout.rightMargin: 12
            }

        }
    }


}
