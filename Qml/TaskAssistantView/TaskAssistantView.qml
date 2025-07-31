import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15

Rectangle {
    id: root

    property var operateMode
    property var rootController
    property string viewName

    color: "transparent"


    Text{
        id: text

        text: "Welcome"

        anchors.centerIn: parent
        width: 100
    }


}
