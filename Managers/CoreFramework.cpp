#include "CoreFramework.h"
#include "DataManager.h"
#include "UserManager.h"
#include "NetworkManager.h"
#include "TaskManager.h"
#include "AccountManager.h"
#include "FeedbackManager.h"

CoreFramework::CoreFramework() {}

void CoreFramework::initialize()
{
    initManagers();
    initAppVersionInfo();
    initAppData();
}

void CoreFramework::initManagers()
{
    mDataManager = std::make_shared<DataManager>(shared_from_this());
    mUserManager = std::make_shared<UserManager>(shared_from_this());
    mNetworkManager = std::make_shared<NetworkManager>(shared_from_this());
    mTaskManager = std::make_shared<TaskManager>(shared_from_this());
    mAccountManager = std::make_shared<AccountManager>(shared_from_this());
    mFeedbackManager = std::make_shared<FeedbackManager>(shared_from_this());

    mTaskManager->initialize();
    mAccountManager->initialize();
    mFeedbackManager->initialize();
}

void CoreFramework::refreshDataFromServer()
{
    mTaskManager->refreshDataFromServer();
    mAccountManager->refreshDataFromServer();
    mFeedbackManager->refreshDataFromServer();
}

void CoreFramework::saveLoginUserInfo(std::string username)
{
    auto userInfos = mAccountManager->getUsers();
    LOG_INFO("saveLoginUserInfo, userInfos: " + std::to_string(userInfos.size()));
    for(auto& userInfo : userInfos)
    {
        if(userInfo.name == username)
        {
            mLoginUserInfo = userInfo;
            LOG_INFO("saveLoginUserInfo, username: " +username);
            return;
        }
    }
    LOG_INFO("Can not find this user info, username: " +username);
}

AppVersionInfo CoreFramework::getVersionInformation()
{
    return mVersionInfo;
}

UserInfo CoreFramework::getLoginUserInfo()
{
    return mLoginUserInfo;
}

void CoreFramework::initAppVersionInfo()
{
    auto result = mNetworkManager->getLatestVersion();
    LOG_INFO(result.statusStr);
    LOG_INFO(result.rawResponse);
    if(result.status == ResultStatus::GetVersionSuccess)
    {
        mVersionInfo = result.versionInfo;
    }
    else
    {
        LOG_INFO("Failed to get app latest version information");
    }
}

void CoreFramework::updateAppToLatestVersion(std::function<void(int)> callback)
{
    std::thread([this, callback]() {
        LOG_INFO("updateAppToLatestVersion");
        mNetworkManager->downloadInstaller(mVersionInfo.downloadUrl, mVersionInfo.clientSavePath, callback);
    }).detach();
}

void CoreFramework::initAppData()
{
    if (!mDataManager || !mNetworkManager) {
        LOG_INFO("DataManager 或 mNetworkManager 为空，无法处理数据");
        return;
    }

    // 使用 std::async 异步运行任务
    auto future = std::async(std::launch::async, [this]() {
        auto result = mNetworkManager->downloadDbFile();
        LOG_INFO(result.statusStr);
        LOG_INFO(result.rawResponse);
        if(result.status == ResultStatus::DatabaseFileDownloadSucess)
        {
            mDataManager->refreshAllDataFromDB();
        }
    });

    std::thread([this, future = std::move(future)]() mutable {
        future.wait();
        LOG_INFO("initAppData 数据初始化完成");
    }).detach();
}

DataManagerPtr CoreFramework::getDataManager()
{
    return mDataManager;
}

UserManagerPtr CoreFramework::getUserManager()
{
    return mUserManager;
}

NetworkManagerPtr CoreFramework::getNetworkManager()
{
    return mNetworkManager;
}

TaskManagerPtr CoreFramework::getTaskManager()
{
    return mTaskManager;
}

AccountManagerPtr CoreFramework::getAccountManager()
{
    return mAccountManager;
}

FeedbackManagerPtr CoreFramework::getFeedbackManager()
{
    return mFeedbackManager;
}
