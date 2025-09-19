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
        LOG_INFO("filePath: " +dataFilePath.toStdString());
        if(dataFilePath.isEmpty())
        {
            LOG_INFO("filePath 为空，无法处理文件：" +dataFilePath.toStdString());
            emit refreshDatabaseFinished();
            return;
        }

        // 使用 QtConcurrent 异步运行长时间任务
        QFuture<void> future = QtConcurrent::run([this, dataManager, dataFilePath]() {
            if(dataManager->refreshAllDataFromFile(dataFilePath))
            {
                LOG_INFO("Refresh DB data by excel file");
            }
        });

        // 连接 QFutureWatcher 以处理任务完成
        disconnect(&mFutureWatcher, nullptr, this, nullptr);
        connect(&mFutureWatcher, &QFutureWatcher<void>::finished, this, [this, networkManager]() {
            auto result = networkManager->uploadDbFile();
            LOG_INFO(result.statusStr);
            LOG_INFO(result.rawResponse);
            refreshDataCount();
            emit refreshDatabaseFinished();
        });

        // 设置 future 以监控任务完成
        mFutureWatcher.setFuture(future);
    }
    else
    {
        LOG_INFO("DataManager 或 NetworkManager 为空，无法处理文件：" +filePath.toStdString());
        emit refreshDatabaseFinished();
    }

}
