#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Controller/Controller.h"
#include "Utils/ControllerUtils.h"

void initQmlComponent()
{
    qmlRegisterType <Controller>("Controller", 1, 0, "Controller");

    std::string qmlDirPath = "C:/PersonalDev/PictureManager/Qml/";
    auto qmlFiles = CUtils::TraversingFilesRecursive(qmlDirPath);
    for (auto& qmlFile : qmlFiles)
    {

        //qmlFile = C:/PersonalDev/PictureManager/Qml/WelcomePage/WelcomePage.qml
        std::string qmlPathName = qmlFile.substr(qmlFile.find("/Qml/") + 5); // WelcomePage/WelcomePage.qml
        std::string qml = qmlFile.substr(qmlFile.rfind(".")); // .qml
        std::string qmlName = qmlFile.substr(qmlFile.rfind("/") + 1); // WelcomePage.qml
        std::string qmlType = qmlName.substr(0, qmlName.length() - qml.length()); // WelcomePage
        cout << qmlPathName << endl;
        qmlRegisterType(QUrl(QString("qrc:/Qml/%1").arg(QString::fromStdString(qmlPathName))), "PictureManager", 1, 0, qmlType.c_str());
    }
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    app.setOrganizationName("Dylan  ");
    app.setOrganizationDomain("dylan.com");
    app.setApplicationName("Class Scheduler");

    //initQmlComponent();
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("ClassScheduler", "Main");

    auto controller = engine.rootObjects().at(0)->findChild<Controller *>("controller");
    //controller->initialize();

    return app.exec();
}
