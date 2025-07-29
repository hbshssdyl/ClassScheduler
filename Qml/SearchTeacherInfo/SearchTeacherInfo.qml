import "../BasicComponent"
import "../JSUtils/ColorUtils.js" as ColorUtils
import "../JSUtils/MainUtils.js" as MainUtils
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Rectangle {
    id: root

    property var operateMode
    property var rootController
    property string viewName
    property var controller: rootController.getSearchTeacherInfoController()
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
            resultsCount: controller.teacherInfoMap.teacherInfoList.length
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

                    model: controller.teacherInfoMap.teacherInfoList
                    spacing: 0
                    width: listView.contentWidth
                    z: 1
                    contentWidth: root.width
                    flickableDirection: Flickable.AutoFlickIfNeeded
                    header: headerView
                    headerPositioning: ListView.OverlayHeader

                    delegate: SearchTeacherInfoDelegate {
                        id: delegateItem

                        maxStudentCount: controller.teacherInfoMap.maxStudentCount
                        width: listView.contentWidth
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
                            id: teacherInfoHeader

                            Repeater {
                                id: repeater

                                model: controller.teacherInfoMap.teacherInfoHeader

                                delegate: Rectangle {
                                    id: repeaterDelegateItem

                                    color: "#66FFFF"
                                    width: MainUtils.getTeacherInfoWidth(root.width, index, controller.teacherInfoMap.teacherInfoHeader.length)
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
