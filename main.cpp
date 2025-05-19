#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "Controller/Controller.h"
#include "Utils/ControllerUtils.h"

void initQmlComponent()
{
    qmlRegisterType <Controller>("Controller", 1, 0, "Controller");

    // std::string qmlDirPath = "C:/PersonalDev/ClassScheduler/Qml/";
    // auto qmlFiles = CUtils::TraversingFilesRecursive(qmlDirPath);
    // for (auto& qmlFile : qmlFiles)
    // {

    //     //qmlFile = C:/PersonalDev/ClassScheduler/Qml/WelcomePage/WelcomePage.qml
    //     std::string qmlPathName = qmlFile.substr(qmlFile.find("/Qml/") + 5); // WelcomePage/WelcomePage.qml
    //     std::string qml = qmlFile.substr(qmlFile.rfind(".")); // .qml
    //     std::string qmlName = qmlFile.substr(qmlFile.rfind("/") + 1); // WelcomePage.qml
    //     std::string qmlType = qmlName.substr(0, qmlName.length() - qml.length()); // WelcomePage
    //     cout << qmlPathName << " " << qmlType << endl;
    //     qmlRegisterType(QUrl(QString("qrc:/Qml/%1").arg(QString::fromStdString(qmlPathName))), "ClassScheduler", 1, 0, qmlType.c_str());
    // }
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
    app.setOrganizationName("Dylan  ");
    app.setOrganizationDomain("dylan.com");
    app.setApplicationName("Class Scheduler");

    // 设置支持自定义的样式（任选其一）
    QQuickStyle::setStyle("Fusion");    // 跨平台风格
    // QQuickStyle::setStyle("Material"); // Material Design风格
    // QQuickStyle::setStyle("Basic");    // 基本风格

    initQmlComponent();
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("ClassScheduler", "Main");

    // auto controller = engine.rootObjects().at(0)->findChild<Controller *>("controller");
    // controller->initialize();

    return app.exec();
}
