import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import "../BasicComponent"
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle {
    id: root

    property var operateMode
    property var rootController
    property var controller: rootController.getSearchClassInfoController()
    property int delegateHeight: 50

    color: "#FFFFFF"
    radius: 5

    border {
        color: "#D6D6D6"
        width: 1
    }

    ColumnLayout{
        id: columnLayout

        anchors.fill: parent

        spacing: 5


        SearchBar{
            id: searchBar

            Layout.topMargin: 5
            Layout.leftMargin: 5
            Layout.alignment: Qt.AlignTop
            Layout.preferredHeight: 50
            Layout.preferredWidth: 400

            onSearchTriggered: function(query) {
                console.debug(controller.classInfoMap.classInfoList.length);
                console.debug(controller.classInfoMap.classInfoList[10].length);
                controller.onSearchTriggered(query);
            }
        }

        Item {
            id: scrollViewItem

            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollView {
                id: scrollView

                anchors.fill: parent

                clip: true

                ListView {
                    id: listView

                    model: controller.classInfoMap.classInfoList.length
                    anchors.fill: parent

                    spacing: 0
                    contentWidth: 19 * 100 + 20
                    flickableDirection: Flickable.AutoFlickIfNeeded
                    header: headerView;
                    headerPositioning: ListView.OverlayHeader;

                    delegate: Rectangle{
                        id: delegateroot

                        property int delegateHeight: 50
                        property int textItemWidth: 100
                        property var repeaterList
                        width: listView.contentWidth
                        height: delegateHeight
                        z: 1
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

                                model: controller.classInfoMap.classInfoList[index]

                                delegate: Rectangle{
                                    id: classInfoItem

                                    color: "red"
                                    width: textItemWidth
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
                                        text: repeater.modelData
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
                    // SearchClassInfoDelegate{
                    //     id: delegateItem

                    //     repeaterList: controller.classInfoMap.classInfoList
                    //     width: listView.contentWidth
                    //     height: delegateHeight
                    //     z: 1
                    // }
                }

                Component {
                    id: headerView
                    Rectangle {
                        width: parent.width
                        height: 50
                        color: "transparent"
                        z: 2

                        Row{
                            id: classInfoHeader

                            Repeater {
                                id: repeater

                                model: controller.classInfoMap.classInfoHeader

                                delegate: Rectangle {
                                    id: repeaterDelegateItem

                                    property int mWidth: {
                                        if(modelData == "序号")
                                            return 50;
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

    }



}
