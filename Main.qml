import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import Controller 1.0
import ClassScheduler 1.0

Window {
    width: 640
    height: 480
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
        spacing: 0

        ActionItems {
            id: actionItems

            controller: controller
            visible: controller.showActions
            Layout.alignment: Qt.AlignTop
            Layout.preferredHeight: 50
            Layout.fillWidth: true
            Layout.topMargin: 12
            Layout.leftMargin: 12
            Layout.rightMargin: 12
        }

        OperateArea{
            id: operateArea

            controller: controller
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 12

        }

    }
}
