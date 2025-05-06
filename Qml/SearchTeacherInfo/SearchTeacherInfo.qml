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

                    model: controller.classInfoList
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
                            id: classInfoHeader

                            Repeater {
                                id: repeater

                                model: controller.classHeaderList

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

    }



}
