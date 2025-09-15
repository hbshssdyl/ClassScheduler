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

void CoreFramework::saveLoginUserInfo(std::string username)
{
    auto userInfos = mAccountManager->getUsers();
    for(auto& userInfo : userInfos)
    {
        if(userInfo.name == username)
        {
            mLoginUserInfo = userInfo;
            return;
        }
    }
    std::cout << "Can not find this user info, username: " << username << std::endl;
}

UserInfo CoreFramework::getLoginUserInfo()
{
    return mLoginUserInfo;
}

void CoreFramework::initAppData()
{
    if (!mDataManager || !mNetworkManager) {
        qWarning() << "DataManager 或 mNetworkManager 为空，无法处理数据";
        return;
    }

    // 使用 std::async 异步运行任务
    auto future = std::async(std::launch::async, [this]() {
        auto result = mNetworkManager->downloadDbFile();
        std::cout << result.statusStr << std::endl;
        std::cout << result.rawResponse << std::endl;
        if(result.status == ResultStatus::DatabaseFileDownloadSucess)
        {
            mDataManager->refreshAllDataFromDB();
        }
    });

    std::thread([this, future = std::move(future)]() mutable {
        future.wait();
        std::cout << "initAppData 数据初始化完成" << std::endl;
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
