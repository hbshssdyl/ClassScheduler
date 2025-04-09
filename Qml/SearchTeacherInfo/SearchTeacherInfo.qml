import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQuick.Window 2.15
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle {
    id: root

    property var operateMode
    property var rootController
    property var controller: rootController.getSearchTeacherInfoController()
    property int minDelegateHeight: 50

    color: "#FFFFFF"
    radius: 5

    border {
        color: "#D6D6D6"
        width: 1
    }

    ScrollView {
        id: scrollView

        anchors.fill: parent
        clip: true

        ListView {
            id: listView

            model: controller.teacherInfoList
            anchors.fill: parent
            spacing: 12

            delegate: SearchTeacherInfoDelegate{
                id: delegateItem

                width: root.width
                height: delegateHeight
                minDelegateHeight: root.minDelegateHeight
            }
        }
    }


}
