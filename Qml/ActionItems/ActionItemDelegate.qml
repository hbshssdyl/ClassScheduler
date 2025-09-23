import "../JSUtils/ColorUtils.js" as ColorUtils
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15

Rectangle {
    id: root

    property var selectedOperateMode

    signal buttonClicked(var mode)

    color: modelData.isSelected ? ColorUtils.getActionItemBackgroundSelectedColor() : ColorUtils.getActionItemBackgroundUnselectedColor()
    radius: 5

    border {
        color: ColorUtils.getActionItemBorderColor()
        width: 1
    }

    RowLayout {
        id: rowLayout

        anchors.fill: parent
        spacing: 2

        Image {
            id: icon

            Layout.preferredHeight: 20
            Layout.preferredWidth: 20
            Layout.leftMargin: 5
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            fillMode: Image.PreserveAspectCrop
            source: "qrc:/ClassScheduler/Resource/" + modelData.actionIcon
        }

        Item {
            id: textItem

            Layout.fillWidth: true
            Layout.preferredHeight: parent.height
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

            Text {
                id: actionName

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 5 // 可选，控制距离
                text: modelData.actionName

                font {
                    bold: false
                    pixelSize: 12
                }

            }

        }

    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        hoverEnabled: true
        onHoveredChanged: {
            if (modelData.isSelected)
                return ;

            if (containsMouse)
                root.color = ColorUtils.getActionItemBackgroundHoverdColor();
            else
                root.color = ColorUtils.getActionItemBackgroundUnselectedColor();
        }
        onPressed: {
            if (modelData.isSelected)
                return ;

            root.color = ColorUtils.getActionItemBackgroundSelectedColor();
        }
        onReleased: {
            if (modelData.isSelected)
                return ;

            if (mouseArea.containsMouse)
                root.buttonClicked(modelData.OperateMode);
            else
                root.color = ColorUtils.getActionItemBackgroundUnselectedColor();
        }
    }

}
