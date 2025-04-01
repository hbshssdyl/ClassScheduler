import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle{
    id: root

    property var selectedOperateMode

    signal buttonClicked(var mode)

    color: modelData.isSelected ? ColorUtils.getActionItemBackgroundSelectedColor() : ColorUtils.getActionItemBackgroundUnselectedColor()
    radius: 5

    border {
        color: ColorUtils.getActionItemBorderColor()
        width: 1
    }

    Text{
        id: actionName

        anchors.centerIn: parent
        text: modelData.actionName
        font {
            bold: false
            pixelSize: 12
        }
    }

    MouseArea{
        id: mouseArea

        anchors.fill: parent
        hoverEnabled: true
        onHoveredChanged:{
            if(modelData.isSelected)
                return;

            if(containsMouse)
                root.color = ColorUtils.getActionItemBackgroundHoverdColor()
            else
                root.color = ColorUtils.getActionItemBackgroundUnselectedColor()
        }

        onPressed: {
            if(modelData.isSelected)
                return;

            root.color = ColorUtils.getActionItemBackgroundSelectedColor()
        }
        onReleased: {
            if(modelData.isSelected)
                return;

            if(mouseArea.containsMouse){
                root.buttonClicked(modelData.OperateMode);
            }
            else
                root.color = ColorUtils.getActionItemBackgroundUnselectedColor()
        }
    }
}
