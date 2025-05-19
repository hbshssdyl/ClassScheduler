import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls.Basic

Item {
    id: searchBar

    property int resultsCount: 0

    width: 400
    height: 50

    signal searchTriggered(string query)

    Rectangle {
        id: background

        anchors.fill: parent
        color: "#f5f5f5"
        radius: 8
        border.color: "#cccccc"

        RowLayout {
            id: layout

            anchors.fill: parent
            spacing: 8
            anchors.margins: 8

            TextField {
                id: searchField

                Layout.fillWidth: true
                placeholderText: "输入搜索的内容，多个关键词用逗号隔开..."
                font.pixelSize: 16
                background: Rectangle {
                    color: "#ffffff"
                    radius: 4
                    border.color: "#cccccc"
                }

                Keys.onReturnPressed: searchButton.click()
                Keys.onEnterPressed: searchButton.click()
            }

            Button {
                id: searchButton

                text: "搜索"
                font.pixelSize: 16
                background: Rectangle {
                    color: "#4CAF50"
                    radius: 4
                }
                contentItem: Text {
                    text: searchButton.text
                    color: "#ffffff"
                    font.pixelSize: 16
                    anchors.centerIn: parent
                }
                onClicked: {
                    searchBar.searchTriggered(searchField.text)
                }
            }
        }
    }

    TextEdit {
        id: resultText

        readOnly: true
        visible: resultsCount > 0
        selectByMouse: true
        height: parent.height
        verticalAlignment: Text.AlignVCenter
        anchors.left: background.right
        anchors.leftMargin: 10

        text: "当前页面共有： " + resultsCount + " 条数据"

        font {
            bold: true
            pixelSize: 16
        }

    }
}
