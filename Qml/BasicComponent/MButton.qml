import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle{
    id: root

    property var borderColor: ColorUtils.getActionItemBorderColor()
    property var backgroundColor: ColorUtils.getActionItemBackgroundUnselectedColor()

    property var pressedColor: ColorUtils.getActionItemBackgroundSelectedColor()
    property var hoverdColor: ColorUtils.getActionItemBackgroundHoverdColor()
    property int borderWidth: 1

    property string text
    property int textPixelSize: 16

    signal clicked()

    color: root.backgroundColor;

    border {
        color: root.borderColor
        width: root.borderWidth
    }

    Text{
        id: buttonTextItem

        anchors.centerIn: parent
        text: root.text
        font {
            bold: false
            pixelSize: 10
        }
    }

    MouseArea{
        id: mouseArea

        anchors.fill: parent
        hoverEnabled: true
        onHoveredChanged:{
            if(containsMouse)
                root.color = root.hoverdColor;
            else
                root.color = root.backgroundColor;
        }

        onPressed: {
            root.color = root.pressedColor;
        }
        onReleased: {
            root.color = root.backgroundColor;
            root.clicked();
        }
    }
}
