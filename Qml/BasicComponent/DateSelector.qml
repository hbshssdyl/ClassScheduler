// DateSelector.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Button {
    id: root
    property date selectedDate: defaultDate
    property date defaultDate: new Date()
    property string format: "yyyy-MM-dd"

    text: Qt.formatDate(selectedDate, format)

    onClicked: popup.open()

    Popup {
        id: popup
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        width: 320
        height: 320

        contentItem: Calendar {
            id: calendar
            anchors.fill: parent
            selectedDate: root.selectedDate
            onSelectedDateChanged: {
                root.selectedDate = selectedDate
                root.text = Qt.formatDate(selectedDate, root.format)
                popup.close()
            }
        }
    }
}
