import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.platform

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "数据库管理页面"

    Column {
        spacing: 20
        anchors.centerIn: parent

        // 显示当前数据库中的数据条数
        Text {
            id: dataCountText
            text: "当前数据库中存在着 0 条数据" // 默认值
            font.pixelSize: 16
        }

        // 提问是否需要提供新的 Excel 文件
        Text {
            text: "是否需要提供新的 Excel 文件来更新当前的数据库？"
            font.pixelSize: 16
        }

        // 文件上传按钮和文件名显示
        Row {
            spacing: 10

            FileDialog {
                id: fileDialog
                title: "选择一个 Excel 文件"
                nameFilters: ["Excel 文件 (*.xlsx *.xls)"]

                onAccepted: {
                    uploadedFileName.text = fileDialog.file
                }
            }

            Button {
                text: "上传文件"
                onClicked: fileDialog.open()
            }

            Text {
                id: uploadedFileName
                text: "未选择文件"
                font.pixelSize: 14
                color: "gray"
            }
        }

        // 完成按钮
        Button {
            text: "完成"
            onClicked: {
                console.log("完成按钮被点击")
                // 在这里添加完成按钮的逻辑
            }
        }
    }
}
