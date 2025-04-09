import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import Qt.labs.folderlistmodel 2.5
import QtQuick.Dialogs 1.3

Rectangle {
    id: root

    property var operateMode
    property var rootController

    color: "#FFFFFF"
    radius: 5

    border {
        color: "#D6D6D6"
        width: 1
    }

    ColumnLayout {
        id: columnLayout

        anchors.fill: parent
        spacing: 5

        Rectangle {
            id: folderPathItem

            Layout.alignment: Qt.AlignTop
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            Layout.topMargin: 2
            Layout.leftMargin: 7
            Layout.rightMargin: 7

            RowLayout {
                id: folderPathItemLayout

                anchors.fill: parent
                spacing: 5

                Button {
                    id: folderSelectAction

                    text: qsTr("选择路径")
                    Layout.fillHeight: true
                    Layout.topMargin: 5
                    Layout.bottomMargin: 5
                    Layout.preferredWidth: 60

                    onClicked: {
                        folderDialog.open()
                    }

                    background: Rectangle {
                        id: buttonBackground

                        anchors.fill: parent
                        color: "#66ffff"
                        radius: 5

                        border {
                            color: "#D6D6D6"
                            width: 1
                        }
                    }
                }

                Item {
                    id: folderPathTextItem

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.topMargin: 5
                    Layout.bottomMargin: 5

                    TextField  {
                        id: folderPathText

                        text: ""
                        color: "#707070"
                        readOnly: true
                        activeFocusOnTab: true
                        selectByMouse: true
                        selectedTextColor: "white"
                        selectionColor: "#4A6DBC"
                        verticalAlignment: TextInput.AlignVCenter
                        horizontalAlignment: TextInput.AlignLeft
                        anchors.fill: parent
                        leftPadding: 10
                        placeholderText: "请选择一个文件夹"
                        font {
                            bold: false
                            pixelSize: 16
                        }

                        background: Rectangle {
                            id: textFieldBackground

                            anchors.fill: parent
                            color: "#F3F6FA"
                            radius: 5

                            border{
                                width: 1
                                color: "#DFE0E4"
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            id: deleteActionItem

            visible: imageListView.visible
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            Layout.leftMargin: 20
            Layout.rightMargin: 20

            color: "#DFE0E4"
            radius: 5

            border{
                width: 1
                color: "#DFE0E4"
            }

            RowLayout {
                id: actionLayout

                anchors.fill: parent
                spacing: 0

                Text {
                    id: textItem

                    color: "#707070"
                    text: "点击“删除”按钮后，红色边框内的图片将会被删除。"
                    //Layout.preferredWidth: true
                    Layout.topMargin: 5
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignCenter

                    font {
                        bold: false
                        pixelSize: 16
                    }
                }

                MButton {
                    id: actionButton

                    text: "删除"
                    textPixelSize: 14
                    radius: height / 2
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 25
                    Layout.rightMargin: 10
                    Layout.alignment: Qt.AlignRight

                    onClicked:{
                        controller.onDeleteRepeatedImagesAction();
                        root.controller.onForderPathReceived(folderDialog.fileUrl + "/");
                    }
                }
            }
        }

        Rectangle{
            id: imagesView

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.bottomMargin: 2
            Layout.leftMargin: 7
            Layout.rightMargin: 7
            clip: true

            ListView {
                id: imageListView

                visible: false
                anchors.fill: parent
                spacing: 0

                model: root.controller.groupRepeatedImages

                delegate: RepeatedImagesDelegate {
                    id: imageDelegate

                    width: root.width - 20
                    height: 150
                }
            }
        }

    }

    FileDialog {
        id: folderDialog

        title: "请选择一个文件夹"
        selectFolder: true
        folder: ""
        onAccepted: {
            var dirPath = fileUrl + "/";
            root.controller.onForderPathReceived(dirPath);
            imageListView.visible = true;
            folderPathText.text = dirPath;
        }
    }
}
