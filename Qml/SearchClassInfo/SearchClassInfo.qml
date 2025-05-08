import "../BasicComponent"
import "../JSUtils/ColorUtils.js" as ColorUtils
import "../JSUtils/MainUtils.js" as MainUtils
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15

Rectangle {
    id: root

    property var operateMode
    property var rootController
    property var controller: rootController.getSearchClassInfoController()
    property int delegateHeight: 50

    color: "#e8f5e9"
    radius: 5
    clip: true

    border {
        color: "#D6D6D6"
        width: 1
    }

    ColumnLayout {
        id: columnLayout

        anchors.fill: parent
        spacing: 5

        SearchBar {
            id: searchBar

            Layout.topMargin: 5
            Layout.leftMargin: 5
            Layout.alignment: Qt.AlignTop
            Layout.preferredHeight: 50
            Layout.preferredWidth: 400
            resultsCount: controller.classInfoMap.classInfoList.length
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

                    model: controller.classInfoMap.classInfoList
                    spacing: 0
                    width: listView.contentWidth
                    z: 1
                    contentWidth: root.width
                    flickableDirection: Flickable.AutoFlickIfNeeded
                    header: headerView
                    headerPositioning: ListView.OverlayHeader

                    delegate: SearchClassInfoDelegate {
                        id: delegateItem

                        width: listView.contentWidth
                        height: delegateHeight
                        rootWidth: root.width
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

                        Row {
                            id: classInfoHeader

                            Repeater {
                                id: repeater

                                model: controller.classInfoMap.classInfoHeader

                                delegate: Rectangle {
                                    id: repeaterDelegateItem

                                    color: "#66FFFF"
                                    width: MainUtils.getClassInfoWidth(root.width, index, controller.classInfoMap.classInfoHeader.length)
                                    height: 50
                                    z: 3

                                    border {
                                        color: ColorUtils.getActionItemBorderColor()
                                        width: 1
                                    }

                                    TextEdit {
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
