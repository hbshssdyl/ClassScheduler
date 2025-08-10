import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button {
    id: root
    text: Qt.formatDate(currentDate, "yyyy-MM-dd")

    property date currentDate: new Date()
    property alias selectedDate: calendar.selectedDate

    onClicked: popup.open()

    Popup {
        id: popup
        anchors.centerIn: Overlay.overlay
        width: 300
        height: 350
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        ColumnLayout {
            anchors.fill: parent
            spacing: 8

            // 标题：显示年月和导航按钮
            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                spacing: 10

                Button {
                    text: "<"
                    onClicked: {
                        var newDate = new Date(calendar.currentDate)
                        newDate.setMonth(calendar.currentDate.getMonth() - 1)
                        calendar.currentDate = newDate
                    }
                }

                Label {
                    text: Qt.formatDate(calendar.currentDate, "MMMM yyyy")
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                Button {
                    text: ">"
                    onClicked: {
                        var newDate = new Date(calendar.currentDate)
                        newDate.setMonth(calendar.currentDate.getMonth() + 1)
                        calendar.currentDate = newDate
                    }
                }
            }

            // 星期标题行
            DayOfWeekRow {
                Layout.fillWidth: true
                locale: Qt.locale("en_US")
                font.pixelSize: 12
                delegate: Label {
                    text: model.shortName
                    color: "#333"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            // 主日历布局：周数列 + 日期网格
            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 4

                // 周数列
                WeekNumberColumn {
                    Layout.fillHeight: true
                    month: calendar.currentDate.getMonth()
                    year: calendar.currentDate.getFullYear()
                    locale: Qt.locale("en_US")
                    font.pixelSize: 12
                    delegate: Label {
                        text: model.weekNumber
                        color: "#666"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                // 日期网格
                MonthGrid {
                    id: calendar
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    month: currentDate.getMonth()
                    year: currentDate.getFullYear()
                    locale: Qt.locale("en_US")
                    property date currentDate: root.currentDate
                    property date selectedDate: currentDate

                    delegate: Rectangle {
                        width: calendar.cellWidth
                        height: calendar.cellHeight
                        color: model.today ? "#e6f3ff" : model.month === calendar.month ? "white" : "#f5f5f5"
                        border.color: "#ddd"
                        border.width: model.today ? 1 : 0

                        Label {
                            anchors.centerIn: parent
                            text: model.day
                            color: model.month === calendar.month ? "#333" : "#999"
                            font.bold: model.today
                        }

                        MouseArea {
                            anchors.fill: parent
                            enabled: model.month === calendar.month
                            onClicked: {
                                calendar.selectedDate = model.date
                                root.currentDate = model.date
                                root.text = Qt.formatDate(model.date, "yyyy-MM-dd")
                                popup.close()
                            }
                        }
                    }
                }
            }
        }
    }
}
