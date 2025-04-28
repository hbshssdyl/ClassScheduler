import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts 1.15

import Qt.labs.platform

Rectangle {
    id: root

    property var rootController

    signal fileUploadFinish(var filePath)

    anchors.fill: parent
    color: "#f5f5f5"

    ColumnLayout {
        id: fileUploadLayout

        anchors.fill: parent
        spacing: 10
        anchors.margins: 20

        // 标题部分
        Rectangle {
            id: titleItem

            Layout.fillWidth: true
            height: 60
            color: "#4CAF50"
            radius: 10

            Text {
                text: "数据更新"
                font.pixelSize: 24
                font.bold: true
                color: "white"
                anchors.centerIn: parent
            }
        }

        // 数据条数显示部分
        Rectangle {
            id: dataItem

            Layout.fillWidth: true
            height: 80
            color: "#ffffff"
            radius: 10
            border.color: "#dddddd"

            ColumnLayout {
                id: dataLayout

                anchors.centerIn: parent
                spacing: 10

                Text {
                    id: currentData

                    Layout.alignment: Qt.AlignCenter
                    text: "当前数据库中存在着以下数据："
                    font.pixelSize: 16
                    color: "#333333"
                }

                Text {
                    id: dataCountText

                    Layout.alignment: Qt.AlignCenter
                    text: rootController.dataCount
                    font.pixelSize: 18
                    font.bold: true
                    color: "#4CAF50"
                }
            }
        }

        // 文件上传部分
        Rectangle {
            id: fileUploadItem

            Layout.fillWidth: true
            height: 120
            color: "#ffffff"
            radius: 10
            border.color: "#dddddd"

            ColumnLayout {
                id: fileUploadItemLayout

                anchors.centerIn: parent
                spacing: 10

                Text {
                    id: fieUploadTextItem

                    text: "是否需要提供新的 Excel 文件来更新当前的数据库？"
                    font.pixelSize: 16
                    color: "#333333"
                    horizontalAlignment: Text.AlignHCenter
                }

                RowLayout {
                    id: fileUploadButtonLayout

                    spacing: 20
                    Layout.alignment: Qt.AlignHCenter

                    Button {
                        id: uploadButton

                        text: "上传文件"
                        font.pixelSize: 14
                        background: Rectangle {
                            id: uploadButtonBg

                            color: uploadButton.hovered ? "#33CCCC" : "#4CAF50"
                            radius: 8
                        }
                        contentItem: Text {
                            id: uploadButtonText

                            text: "上传文件"
                            color: "white"
                            font.pixelSize: 14
                            anchors.centerIn: parent
                        }
                        onClicked: fileDialog.open()
                    }

                    Text {
                        id: uploadedFileName

                        text: "未选择文件"
                        font.pixelSize: 14
                        color: "#888888"
                    }

                    FileDialog {
                        id: fileDialog

                        title: "选择一个 Excel 文件"
                        nameFilters: ["Excel 文件 (*.xlsx *.xls)"]

                        onAccepted: {
                            var name = String(fileDialog.file);
                            uploadedFileName.text = name.substring(name.lastIndexOf("/") + 1);
                        }
                    }
                }
            }
        }

        // 完成按钮部分
        Rectangle {
            id: finishButtonItem

            Layout.fillWidth: true
            height: 60
            color: finishButton.hovered ? "#2196A3" : "#2196F3"
            radius: 10

            Button {
                id: finishButton

                anchors.fill: parent
                //text: "完成"
                font.pixelSize: 16
                background: Rectangle {
                    color: finishButton.hovered ? "#2196A3" : "#2196F3"
                    radius: 8
                }
                Text {
                    id: finishButtonText

                    text: "完成"
                    color: "white"
                    font.pixelSize: 16
                    anchors.centerIn: parent
                }
                onClicked: {
                    var filePath = String(fileDialog.file);
                    root.fileUploadFinish(filePath);
                }
            }

        }
    }
}
