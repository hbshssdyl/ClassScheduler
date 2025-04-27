import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Qt.labs.platform

Rectangle {
    visible: true
    width: 600
    height: 500

    Rectangle {
        anchors.fill: parent
        color: "#f5f5f5"

        ColumnLayout {
            anchors.fill: parent
            spacing: 10
            anchors.margins: 20

            // 标题部分
            Rectangle {
                Layout.fillWidth: true
                height: 60
                color: "#4CAF50"
                radius: 10

                Text {
                    text: "数据库管理"
                    font.pixelSize: 24
                    font.bold: true
                    color: "white"
                    anchors.centerIn: parent
                }
            }

            // 数据条数显示部分
            Rectangle {
                Layout.fillWidth: true
                height: 80
                color: "#ffffff"
                radius: 10
                border.color: "#dddddd"

                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 10

                    Text {
                        text: "当前数据库中存在着以下数据："
                        font.pixelSize: 16
                        color: "#333333"
                    }

                    Text {
                        id: dataCountText
                        text: "0 条数据" // 默认值
                        font.pixelSize: 18
                        font.bold: true
                        color: "#4CAF50"
                    }
                }
            }

            // 文件上传部分
            Rectangle {
                Layout.fillWidth: true
                height: 120
                color: "#ffffff"
                radius: 10
                border.color: "#dddddd"

                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 10

                    Text {
                        text: "是否需要提供新的 Excel 文件来更新当前的数据库？"
                        font.pixelSize: 16
                        color: "#333333"
                        horizontalAlignment: Text.AlignHCenter
                    }

                    RowLayout {
                        spacing: 20
                        Layout.alignment: Qt.AlignHCenter

                        Button {
                            text: "上传文件"
                            font.pixelSize: 14
                            background: Rectangle {
                                color: "#4CAF50"
                                radius: 8
                            }
                            contentItem: Text {
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
                                uploadedFileName.text = fileDialog.file
                            }
                        }
                    }
                }
            }

            // 完成按钮部分
            Rectangle {
                Layout.fillWidth: true
                height: 60
                color: "#2196F3"
                radius: 10

                Button {
                    anchors.centerIn: parent
                    text: "完成"
                    font.pixelSize: 16
                    background: Rectangle {
                        color: "#2196F3"
                        radius: 8
                    }
                    contentItem: Text {
                        text: "完成"
                        color: "white"
                        font.pixelSize: 16
                        anchors.centerIn: parent
                    }
                    onClicked: {
                        console.log("完成按钮被点击")
                        // 在这里添加完成按钮的逻辑
                    }
                }
            }
        }
    }
}
