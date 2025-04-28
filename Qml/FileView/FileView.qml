import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root

    property var rootController
    //property string currentDataCount: rootController.dataCount

    Loader {
        id: pageLoader

        anchors.fill: parent
        source: "FileUploadView.qml"

        onLoaded: {
            if (item && item.hasOwnProperty("fileUploadFinish")) {
                item.fileUploadFinish.connect(function(filePath) {
                    console.debug(filePath);
                    rootController.onFileUploaded(filePath);
                    pageLoader.source = "FileProcessingView.qml";
                });
            }
        }

        Binding {
            target: pageLoader.item
            property: "rootController"
            value: root.rootController
        }
    }
}
