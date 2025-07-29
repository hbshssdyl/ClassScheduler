import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root

    property var rootController
    property string viewName

    onRootControllerChanged: {
        if (rootController) {
            rootController.refreshDatabaseFinished.connect(function() {
                pageLoader.source = "FileUploadView.qml";
            });
        }
    }

    Loader {
        id: pageLoader

        anchors.fill: parent
        source: "FileUploadView.qml"

        onLoaded: {
            if (item && item.hasOwnProperty("fileUploadFinish")) {
                item.fileUploadFinish.connect(function(filePath) {
                    console.debug(filePath);
                    pageLoader.source = "FileProcessingView.qml";
                    rootController.onFileUploaded(filePath);
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
