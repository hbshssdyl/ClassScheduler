import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import "../JSUtils/ColorUtils.js" as ColorUtils

Rectangle {
    id: root

    property var controller
    property var cachedItems: ({})   // 缓存创建好的页面 Item

    onControllerChanged: {
        if (controller) {
            controller.updateOperateMode.connect(function(mode) {
                root.switchTo(mode);
            });
        }
    }

    color: ColorUtils.getOperateAreaBackgroundColor()
    radius: 5

    border {
        color: ColorUtils.getOperateAreaBorderColor()
        width: 1
    }

    Item {
        id: container
        anchors.fill: parent
    }

    Component.onCompleted: {
        switchTo("LoginView");
    }

    function switchTo(viewName) {
        console.debug("Switch page to: ", viewName);
        let targetUrl = "../" + viewName + "/" + viewName + ".qml";

        // 如果已经加载过这个页面
        if (cachedItems[viewName]) {
            for (let key in cachedItems) {
                cachedItems[key].visible = false;
            }
            cachedItems[viewName].visible = true;
            console.debug("Reuse page: ", viewName);
            return;
        }

        // 第一次加载
        let component = Qt.createComponent(targetUrl);
        if (component.status === Component.Ready) {
            let item = component.createObject(container, {
                rootController: controller
            });
            if (item) {
                item.anchors.fill = container;
                item.visible = true;
                // 隐藏其他页面
                for (let key in cachedItems) {
                    cachedItems[key].visible = false;
                }
                cachedItems[viewName] = item;
                console.debug("New page: ", viewName);
            } else {
                console.error("Create page failed: " + component.errorString());
            }
        } else if (component.status === Component.Error) {
            console.error("Load page failed: " + component.errorString());
        }
    }
}
