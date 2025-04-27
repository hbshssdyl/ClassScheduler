import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle {
    id: root

    property var operateMode
    property var rootController
    property var controller: rootController.getSearchTeacherInfoController()
    property int delegateHeight: 50

    color: "#FFFFFF"
    radius: 5

    border {
        color: "#D6D6D6"
        width: 1
    }



    ScrollView {
        id: scrollView

        anchors.fill: parent

        clip: true

        ListView {
            id: listView

            model: controller.teacherInfoList
            anchors.fill: parent
            spacing: 0
            contentWidth: 19 * 100 + 20
            flickableDirection: Flickable.AutoFlickIfNeeded
            header: headerView;
            headerPositioning: ListView.OverlayHeader;

            delegate: SearchTeacherInfoDelegate{
                id: delegateItem

                width: listView.contentWidth
                height: delegateHeight
                z: 1
            }
        }

        Component {
            id: headerView
            Rectangle {
                width: parent.width
                height: 50
                color: "transparent"
                z: 2

                Row{
                    id: teacherInfoHeader

                    Repeater {
                        id: repeater

                        model: controller.teacherHeaderList

                        delegate: Rectangle {
                            id: repeaterDelegateItem

                            property int mWidth: {
                                if(modelData == "序号")
                                    return 50;
                                if(modelData == "学校")
                                    return 170;
                                return 100;
                            }
                            color: "#66FFFF"
                            width: mWidth
                            height: 50
                            z: 2

                            border {
                                color: ColorUtils.getActionItemBorderColor()
                                width: 1
                            }

                            TextEdit{
                                id: headerText

                                readOnly: true
                                selectByMouse: true
                                anchors.centerIn: parent
                                text: modelData
                                font {
                                    bold: false
                                    pixelSize: 12
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
