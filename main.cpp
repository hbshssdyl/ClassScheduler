#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include "Controller/Controller.h"
#include "Managers/Logger.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void initQmlComponent()
{
    qmlRegisterType<Controller>("Controller", 1, 0, "Controller");
    LOG_INFO("QML Component registered");
}


QString getAppDir()
{
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);
    QFileInfo fi(QString::fromWCharArray(path));
    return fi.absolutePath();
}

int main(int argc, char *argv[])
{
    try
    {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

        // 初始化日志
        Logger::instance().init("application.log", LOG_INFO, LOG_DEBUG);
        LOG_INFO("Logger 已启动");

        // LOG_INFO("ApplicationDirPath: " + QCoreApplication::applicationDirPath().toStdString());

        QString appDir = getAppDir();
        QString qmlModulePath = QDir(appDir).filePath("qml");
        qputenv("QML2_IMPORT_PATH", qmlModulePath.toUtf8());
        LOG_INFO("Set QML2_IMPORT_PATH: " + qmlModulePath.toStdString());


        // 打印启动信息到额外文件（双保险）
        QFile debugLog(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/startup_debug.log");
        if (debugLog.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            QTextStream ts(&debugLog);
            ts << "Main() start" << Qt::endl;
        }

        // 强制 Qt 插件路径
        QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/platforms");
        LOG_INFO("Added platforms path: " + QCoreApplication::applicationDirPath().toStdString() + "/platforms");

        QApplication app(argc, argv);
        LOG_INFO("QApplication created");

        app.setOrganizationName("Dylan");
        app.setOrganizationDomain("dylan.com");
        app.setApplicationName("ClassScheduler");

        // 设置样式
        QQuickStyle::setStyle("Fusion");
        LOG_INFO("QQuickStyle set to Fusion");

        // 注册 QML 组件
        initQmlComponent();

        // 加载 QML
        QQmlApplicationEngine engine;

        QObject::connect(
            &engine,
            &QQmlApplicationEngine::objectCreationFailed,
            &app,
            []() {
                LOG_ERROR("QQmlApplicationEngine: objectCreationFailed");
                QCoreApplication::exit(-1);
            },
            Qt::QueuedConnection
            );

        LOG_INFO("Loading QML module...");

        QObject::connect(&engine, &QQmlApplicationEngine::warnings, [](const QList<QQmlError> &warnings) {
            for (const auto &warning : warnings)
                LOG_ERROR("QML Error: " + warning.toString().toStdString());
        });

        engine.loadFromModule("ClassScheduler", "Main");
        if (engine.rootObjects().isEmpty())
        {
            LOG_ERROR("Engine rootObjects is empty after load");
            return -1;
        }

        LOG_INFO("Entering app.exec()");
        return app.exec();
    }
    catch (const std::exception &e)
    {
        LOG_ERROR(std::string("Exception caught: ") + e.what());
        return -1;
    }
    catch (...)
    {
        LOG_ERROR("Unknown exception caught");
        return -1;
    }
}
