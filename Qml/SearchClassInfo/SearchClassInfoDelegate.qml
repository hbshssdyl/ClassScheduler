import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import "../JSUtils/ColorUtils.js" as ColorUtils
import "../JSUtils/MainUtils.js" as MainUtils


Rectangle{
    id: delegateroot

    property int delegateHeight: 50
    property int rootWidth

    color: index % 2 == 0 ? "transparent" : "#DDDDDD"
    radius: 5

    border {
        color: ColorUtils.getActionItemBorderColor()
        width: 1
    }

    Row{
        id: classInfo

        Repeater {
            id: repeater

            model: modelData

            delegate: Rectangle{
                id: classInfoItem

                color: "transparent"
                width: MainUtils.getClassInfoWidth(rootWidth, index, controller.classInfoMap.classInfoHeader.length)
                height: delegateHeight

                border {
                    color: ColorUtils.getActionItemBorderColor()
                    width: 1
                }

                TextEdit{
                    id: classInfoText

                    selectByMouse: true
                    readOnly: true
                    anchors.centerIn: parent
                    width: parent.width
                    text: modelData
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 5
                    rightPadding: 5
                    wrapMode: TextEdit.WordWrap
                    font {
                        bold: false
                        pixelSize: 12
                    }
                }
            }
        }
    }
}
