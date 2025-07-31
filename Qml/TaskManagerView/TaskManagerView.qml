import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    width: 800
    height: 1000
    color: "#f5f5f5"

    property string currentCategory: "today"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20

        RowLayout {
            Layout.fillWidth: true

            Text {
                text: "Task Review"
                font.pixelSize: 28
                font.bold: true
                color: "#333"
            }

            Item { Layout.fillWidth: true }
        }

        RowLayout {
            spacing: 10
            Layout.fillWidth: true

            Button { text: "Today"; onClicked: currentCategory = "today" }
            Button { text: "This Week"; onClicked: currentCategory = "week" }
            Button { text: "Recurring"; onClicked: currentCategory = "recurring" }
        }

        Rectangle {
            height: 2
            color: "#0096a6"
            Layout.fillWidth: true
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Column {
                width: parent.width
                spacing: 20

                Repeater {
                    model: ListModel {
                        ListElement { title: "Today’s Task"; desc: "Description..."; publish: "Apr 24, 2024"; due: "Apr 24, 2024"; rating: "3"; status: "In Progress"; category: "today" }
                        ListElement { title: "Weekly Review"; desc: "Description..."; publish: "Apr 20, 2024"; due: "Apr 26, 2024"; rating: "4"; status: "Not Started"; category: "week" }
                        ListElement { title: "Monthly Report"; desc: "Description..."; publish: "Apr 01, 2024"; due: "Apr 30, 2024"; rating: "5"; status: "Completed"; category: "recurring" }
                    }

                    delegate: Rectangle {
                        visible: model.category === currentCategory
                        width: parent.width
                        radius: 12
                        color: "white"
                        border.color: "#ddd"
                        border.width: 1
                        implicitHeight: contentLayout.implicitHeight + 32

                        ColumnLayout {
                            id: contentLayout

                            anchors.margins: 16
                            anchors.fill: parent
                            spacing: 10

                            Text {
                                text: model.title
                                font.pixelSize: 20
                                font.bold: true
                                color: "#222"
                                wrapMode: Text.Wrap
                            }

                            Text {
                                text: model.desc
                                font.pixelSize: 14
                                color: "#555"
                                wrapMode: Text.Wrap
                            }

                            RowLayout {
                                spacing: 10
                                Text { text: "Published: " + model.publish; font.pixelSize: 13; color: "#777" }
                                Text { text: "|"; font.pixelSize: 13; color: "#777" }
                                Text { text: "Due: " + model.due; font.pixelSize: 13; color: "#777" }
                            }

                            RowLayout {
                                spacing: 10
                                Label { text: "重要程度:" }

                                Repeater {
                                    model: 5
                                    delegate: Text {
                                        text: index < parseInt(model.rating) ? "★" : "☆"
                                        font.pixelSize: 18
                                        color: "#ff9900"
                                    }
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 10

                                TextArea {
                                    placeholderText: "督导意见"
                                    Layout.fillWidth: true
                                    implicitHeight: 60
                                    wrapMode: TextArea.Wrap
                                    background: Rectangle {
                                        radius: 6
                                        border.color: "#bbb"
                                    }
                                }

                                ColumnLayout {
                                    spacing: 6
                                    Text { text: "评分:"; font.pixelSize: 14; color: "#333" }
                                    ComboBox {
                                        model: ["1", "2", "3", "4", "5"]
                                        currentIndex: parseInt(model.rating) - 1
                                        width: 80
                                    }
                                }

                                Button {
                                    text: "点评提交"
                                    background: Rectangle {
                                        color: "#0096a6"
                                        radius: 6
                                    }
                                    contentItem: Text {
                                        text: "点评提交"
                                        color: "white"
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                    padding: 6
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
