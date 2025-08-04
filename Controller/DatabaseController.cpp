#include "DatabaseController.h"
#include "Managers/DataManager.h"
#include "Managers/NetworkManager.h"

DatabaseController::DatabaseController(CoreFrameworkPtr coreFramework, QObject *parent)
    : mCoreFramework(coreFramework)
    , QObject{parent}
{
    initialize();
}

void DatabaseController::initialize()
{
    refreshDataCount();
}

void DatabaseController::refreshDataCount()
{
    auto coreFramework = mCoreFramework.lock();
    if(auto dataManager = coreFramework->getDataManager())
    {
        mDataCount = QString::number(dataManager->getTableDataCount(CLASS_INFOS_TABLE_NAME));
        dataManager->closeDBConnection();
        emit dataCountChanged();
    }
}

void DatabaseController::onFileUploaded(QString filePath)
{
    auto coreFramework = mCoreFramework.lock();
    auto dataManager = coreFramework->getDataManager();
    auto networkManager = coreFramework->getNetworkManager();

    if(dataManager && networkManager)
    {
        auto dataFilePath = QUrl(filePath).toLocalFile();
        std::cout << "filePath: " << dataFilePath.toStdString() << std::endl;
        if(dataFilePath.isEmpty())
        {
            qWarning() << "filePath 为空，无法处理文件：" << dataFilePath;
            emit refreshDatabaseFinished();
            return;
        }

        // 使用 QtConcurrent 异步运行长时间任务
        QFuture<void> future = QtConcurrent::run([this, dataManager, dataFilePath]() {
            if(dataManager->refreshAllDataFromFile(dataFilePath))
            {
                std::cout << "Refresh DB data by excel file" << std::endl;
            }
        });

        // 连接 QFutureWatcher 以处理任务完成
        disconnect(&mFutureWatcher, nullptr, this, nullptr);
        connect(&mFutureWatcher, &QFutureWatcher<void>::finished, this, [this, networkManager]() {
            std::cout << "onFileUploaded 文件处理完成" << std::endl;
            auto result = networkManager->uploadDbFile();
            std::cout << result.statusStr << std::endl;
            std::cout << result.rawResponse << std::endl;
            emit refreshDatabaseFinished();
        });

        // 设置 future 以监控任务完成
        mFutureWatcher.setFuture(future);
    }
    else
    {
        qWarning() << "DataManager 或 NetworkManager 为空，无法处理文件：" << filePath;
        emit refreshDatabaseFinished();
    }

}
