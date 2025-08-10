import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Button {
    id: root

    property string defaultText
    property string checkedText: defaultText
    property string unCheckedText: defaultText
    property color checkedTextColor: "white"
    property color unCheckedTextColor: "black"
    property color checkedColor: "#0096a6"
    property color downColor: "#d0d0d0"
    property color normalColor: "#f0f0f0"
    property bool defaultChecked: false

    text: checked ? checkedText : unCheckedText
    checkable: true
    contentItem: Text {
        text: root.checked ? checkedText : unCheckedText
        color: root.checked ? checkedTextColor : unCheckedTextColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    background: Rectangle {
        color: parent.checked ? checkedColor : parent.down ? downColor : normalColor
        radius: 4
        border.width: 1
    }
}
