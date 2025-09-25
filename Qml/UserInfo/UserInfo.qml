import "../JSUtils/ColorUtils.js" as ColorUtils
import "../BasicComponent"
import Qt5Compat.GraphicalEffects
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    property var controller
    property string username: "User"
    property bool hasUpdate: getVersionInfoListValue("shouldUpdate", false)
    property string latestVersion: getVersionInfoListValue("latestVersion", "1.0.1") // 最新版本号
    property string currentVersion: getVersionInfoListValue("currentVersion", "1.0.1") // 当前版本号
    property string releaseNotes: getVersionInfoListValue("changeLog", "修复已知问题") // 更新内容

    function getVersionInfoListValue(key, defaultValue)
    {
        if(controller)
        {
            if(controller.versionInfoList[key] !== undefined)
            {
                return controller.versionInfoList[key];
            }
        }
        return defaultValue;
    }

    // 模拟下载过程
    function simulateDownload() {
        var progress = 0;
        var timer = Qt.createQmlObject('import QtQuick 2.15; Timer { interval: 50; repeat: true; running: true }', root);
        timer.triggered.connect(function() {
            progress += 2;
            downloadProgress.value = controller.downloadValue;
            if (progress >= 100) {
                timer.stop();
                timer.destroy();
                // 下载完成，显示安装按钮
                btnInstall.visible = true;
            }
        });
    }

    color: ColorUtils.getOperateAreaBackgroundColor()

    Rectangle {
        id: avatar

        anchors.fill: parent
        color: "transparent"
        clip: true

        Image {
            id: avatarImage

            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop
            source: "qrc:/ClassScheduler/Resource/Banner-mountain.jpg"
        }

        // 数据更新按钮（使用封装好的 LoadingIconButton）
        RefreshButton {

            id: dataUpdateBtn
            iconSource: "qrc:/ClassScheduler/Resource/sync_orange.svg"
            finished: controller.refreshFinished

            anchors {
                top: parent.top
                right: updateIndicator.visible ? updateIndicator.left : parent.right
                rightMargin: 10
                topMargin: 10
            }

            onClicked: {
                controller.updateAllData();
            }
        }

        // 更新提示图标 - 更美观的设计
        Rectangle {
            id: updateIndicator

            visible: hasUpdate
            width: 26
            height: 26
            radius: 13
            color: "#FF6D00" // 橙色更柔和
            // 发光效果
            layer.enabled: true

            anchors {
                top: parent.top
                right: parent.right
                margins: 10
            }

            Image {
                anchors.centerIn: parent
                source: "qrc:/ClassScheduler/Resource/update.svg"
                sourceSize: Qt.size(20, 20)
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: updateDialog.open()
            }

            layer.effect: Glow {
                samples: 16
                color: "#FF6D00"
                spread: 0.4
            }

            // 脉动动画
            SequentialAnimation on scale {
                running: visible
                loops: Animation.Infinite

                NumberAnimation {
                    from: 1
                    to: 1.2
                    duration: 800
                    easing.type: Easing.InOutQuad
                }

                NumberAnimation {
                    from: 1.2
                    to: 1
                    duration: 800
                    easing.type: Easing.InOutQuad
                }

            }

        }

        // Blurred background for the text
        Rectangle {
            id: textBackground

            width: welcomeText.width + 20
            height: welcomeText.height + 20
            radius: 8

            anchors {
                left: parent.left
                leftMargin: 5
                verticalCenter: parent.verticalCenter
            }

            // Create a blurred background using the image
            ShaderEffectSource {
                id: blurSource

                sourceItem: avatarImage
                sourceRect: Qt.rect(textBackground.x, textBackground.y, textBackground.width, textBackground.height)
            }

            FastBlur {
                anchors.fill: parent
                source: blurSource
                radius: 32
                transparentBorder: true
            }

            // Semi-transparent overlay
            Rectangle {
                anchors.fill: parent
                color: "#40000000"
                radius: parent.radius
            }

            // Welcome text
            Text {
                id: welcomeText

                anchors.centerIn: parent
                text: "欢迎 " + controller.name
                color: "white"
                renderType: Text.NativeRendering

                font {
                    pixelSize: 14
                    bold: true
                }

            }

            MouseArea {
                anchors.fill: parent
                onClicked: settingsMenu.open()
            }

        }

    }

    Menu {
        id: settingsMenu

        width: 200
        y: avatar.height

        MenuItem {
            text: "个人设置"
            onTriggered: console.log("个人设置点击")
        }

        MenuItem {
            text: "账户管理"
            onTriggered: console.log("账户管理点击")
        }

        MenuSeparator {
        }

        MenuItem {
            text: "退出"
            onTriggered: console.log("退出点击")
        }

    }

    // 更新对话框 - 更美观的设计
    Popup {
        id: updateDialog

        width: 400
        height: 420
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        anchors.centerIn: Overlay.overlay

        background: Rectangle {
            color: "#FFFFFF"
            radius: 12
            layer.enabled: true

            layer.effect: DropShadow {
                transparentBorder: true
                color: "#80000000"
                radius: 16
                samples: 17
            }

        }

        contentItem: ColumnLayout {
            spacing: 0

            // 标题栏
            Rectangle {
                Layout.fillWidth: true
                height: 50
                color: "#F5F7FA"
                radius: 12

                Text {
                    anchors.centerIn: parent
                    text: "发现新版本"
                    font.pixelSize: 18
                    font.bold: true
                    color: "#333333"
                }

            }

            // 内容区域
            ColumnLayout {
                Layout.topMargin: 20
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                spacing: 15

                // 版本信息
                RowLayout {
                    Layout.fillWidth: true

                    Text {
                        text: "当前版本:"
                        font.pixelSize: 14
                        color: "#666666"
                    }

                    Text {
                        text: currentVersion
                        font.pixelSize: 14
                        color: "#333333"
                        Layout.fillWidth: true
                    }

                }

                RowLayout {
                    Layout.fillWidth: true

                    Text {
                        text: "最新版本:"
                        font.pixelSize: 14
                        color: "#666666"
                    }

                    Text {
                        text: latestVersion
                        font.pixelSize: 14
                        color: "#FF6D00"
                        font.bold: true
                        Layout.fillWidth: true
                    }

                }

                // 更新内容标题
                Text {
                    text: "更新内容:"
                    font.pixelSize: 14
                    color: "#666666"
                    font.bold: true
                }

                // 更新内容
                Rectangle {
                    Layout.fillWidth: true
                    height: 150
                    color: "#F9F9F9"
                    radius: 6
                    border.color: "#EEEEEE"
                    border.width: 1

                    ScrollView {
                        anchors.fill: parent
                        anchors.margins: 5

                        TextArea {
                            readOnly: true
                            text: releaseNotes
                            wrapMode: Text.Wrap
                            background: null
                            color: "#444444"
                            font.pixelSize: 13
                            selectByMouse: true
                        }

                    }

                }

                // 进度条
                ProgressBar {
                    id: downloadProgress

                    visible: false
                    Layout.fillWidth: true
                    from: 0
                    to: 100
                    value: controller.downloadValue
                    onValueChanged: {
                        if (value >= 100)
                            btnInstall.visible = true;

                    }

                    background: Rectangle {
                        implicitHeight: 6
                        color: "#E0E0E0"
                        radius: 3
                    }

                    contentItem: Item {
                        implicitHeight: 6

                        Rectangle {
                            width: downloadProgress.visualPosition * parent.width
                            height: parent.height
                            radius: 3

                            gradient: Gradient {
                                GradientStop {
                                    position: 0
                                    color: "#4CAF50"
                                }

                                GradientStop {
                                    position: 1
                                    color: "#2E7D32"
                                }

                            }

                        }

                    }

                }

                // 按钮区域
                RowLayout {
                    Layout.topMargin: 10
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 15

                    Button {
                        id: btnLater

                        text: "稍后提醒"
                        visible: !downloadProgress.visible
                        onClicked: updateDialog.close()

                        background: Rectangle {
                            color: "transparent"
                            radius: 6
                            border.color: "#CCCCCC"
                            border.width: 1
                        }

                        contentItem: Text {
                            text: btnLater.text
                            color: "#666666"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                    }

                    Button {
                        id: btnUpdate

                        text: "立即更新"
                        visible: !downloadProgress.visible
                        onClicked: {
                            downloadProgress.visible = true;
                            btnLater.visible = false;
                            btnUpdate.visible = false;
                            controller.onUpdateStarted();
                        }

                        background: Rectangle {
                            color: "#FF6D00"
                            radius: 6
                        }

                        contentItem: Text {
                            text: btnUpdate.text
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.bold: true
                        }

                    }

                    Button {
                        id: btnInstall

                        text: "立即安装"
                        visible: false
                        onClicked: {
                            console.log("开始安装新版本...");
                            controller.installLatestVersion();
                        }

                        background: Rectangle {
                            color: "#4CAF50"
                            radius: 6
                        }

                        contentItem: Text {
                            text: btnInstall.text
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.bold: true
                        }

                    }

                }

            }

        }

    }

}
