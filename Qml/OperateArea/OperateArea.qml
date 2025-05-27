import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle {
    id: root

    property var controller

    color: ColorUtils.getOperateAreaBackgroundColor()
    radius: 5

    border {
        color: ColorUtils.getOperateAreaBorderColor()
        width: 1
    }

    RowLayout{
        id: rowLayout
        anchors.fill: parent
        spacing: 0

        Loader {
            id: viewLoader

            property string sourceUrl: "../" + controller.loadedView + "/" + controller.loadedView + ".qml"
            source: sourceUrl
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true
            Layout.fillHeight: true
            onLoaded: {
                item.rootController = root.controller;
            }
        }
    }
}
