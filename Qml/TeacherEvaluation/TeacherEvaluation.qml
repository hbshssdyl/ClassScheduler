import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15

Rectangle {
    id: root

    property var operateMode
    property var rootController
    property string viewName

    color: "#FFFFFF"
    radius: 5

    border {
        color: "#D6D6D6"
        width: 1
    }

    // RowLayout{
    //     id: rowLayout
    //     anchors.fill: parent
    //     spacing: 0

    //     Rectangle{
    //         id: leftArea

    //         Layout.alignment: Qt.AlignLeft
    //         Layout.preferredWidth: parent.width / 2 - 10
    //         Layout.fillHeight: true
    //         Layout.margins: 5
    //         color: "#FFFFFF"
    //         radius: 5

    //         border {
    //             color: "#D6D6D6"
    //             width: 1
    //         }

    //     }
    //     Rectangle{
    //         id: rightArea

    //         Layout.alignment: Qt.AlignLeft
    //         Layout.preferredWidth: parent.width / 2 - 10
    //         Layout.fillHeight: true
    //         Layout.margins: 5
    //         color: "#FFFFFF"
    //         radius: 5

    //         border {
    //             color: "#D6D6D6"
    //             width: 1
    //         }

    //     }
    // }
    ListView {
            id: listView
            anchors.fill: parent
            spacing: 5

            model: ListModel {
                ListElement { name: "张三"; age: 18 }
                ListElement { name: "李四"; age: 19 }
                ListElement { name: "王五"; age: 20 }
            }

            reuseItems: false
            cacheBuffer: 500

            delegate: Item {
                id: delegateRoot
                width: listView.width
                height: contentColumn.implicitHeight + 10  // ✨ 关键：绑定高度

                property bool expanded: false

                Rectangle {
                    width: parent.width
                    height: parent.height
                    color: "#f0f0f0"
                    radius: 5
                    border.color: "#ccc"
                    anchors.fill: parent

                    Column {
                        id: contentColumn
                        anchors.margins: 10
                        anchors.fill: parent
                        spacing: 5

                        Text {
                            text: "姓名: " + model.name
                            font.pixelSize: 16
                        }

                        Button {
                            text: expanded ? "收起详情" : "展开详情"
                            onClicked: expanded = !expanded
                        }

                        // 可展开的内容
                        Column {
                            visible: expanded
                            spacing: 2

                            Text { text: "年龄: " + model.age }
                            Text { text: "课程数量: 随机数据" }
                            Text { text: "详细成绩分析：..." }
                        }
                    }
                }
            }
        }
}
