import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: taskItem
    property string title: "Task"
    property string description: "Description..."
    property string published: "Date"
    property string due: "Date"
    property int rating: 0

    width: parent ? parent.width - 20 : 360
    height: column.implicitHeight + 20
    color: "#FFFFFF" // 白色卡片背景
    radius: 5
    border.color: "#E0E0E0"
    border.width: 1

    ColumnLayout {
        id: column
        anchors.fill: parent
        anchors.margins: 10
        spacing: 5

        Text {
            text: title
            font.pixelSize: 18
            font.bold: true
            color: "#333333"
        }

        Text {
            text: description
            font.pixelSize: 14
            color: "#666666"
        }

        Text {
            text: "Published: " + published + " | Due: " + due
            font.pixelSize: 12
            color: "#757575"
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            TextField {
                placeholderText: "Write comment"
                Layout.fillWidth: true
                background: Rectangle {
                    color: "#F5F5F5"
                    radius: 5
                }
            }

            ComboBox {
                id: ratingCombo
                model: [0, 1, 2, 3, 4, 5]
                currentIndex: rating
                displayText: "Rating: " + (rating > 0 ? rating : "Review")
                background: Rectangle {
                    color: "#F5F5F5"
                    radius: 5
                }
            }
        }
    }
}
