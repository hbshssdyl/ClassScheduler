#ifndef COREFRAMEWORK_H
#define COREFRAMEWORK_H

#include "Utils/DataUtils.h"

using namespace ClassScheduler;
using CoreFrameworkPtr = std::shared_ptr<class CoreFramework>;
using DataManagerPtr = std::shared_ptr<class DataManager>;
using UserManagerPtr = std::shared_ptr<class UserManager>;
using NetworkManagerPtr = std::shared_ptr<class NetworkManager>;
using TaskManagerPtr = std::shared_ptr<class TaskManager>;
using AccountManagerPtr = std::shared_ptr<class AccountManager>;
using FeedbackManagerPtr = std::shared_ptr<class FeedbackManager>;
using StudentGradeManagerPtr = std::shared_ptr<class StudentGradeManager>;

class CoreFramework : public std::enable_shared_from_this<CoreFramework>
{
public:
    CoreFramework();
    void initialize();
    void refreshDataFromServer();
    void saveLoginUserInfo(std::string username);
    void updateAppToLatestVersion(std::function<void(int)> callback);
    AppVersionInfo getVersionInformation();
    UserInfo getLoginUserInfo();
    DataManagerPtr getDataManager();
    UserManagerPtr getUserManager();
    NetworkManagerPtr getNetworkManager();
    TaskManagerPtr getTaskManager();
    AccountManagerPtr getAccountManager();
    FeedbackManagerPtr getFeedbackManager();
    StudentGradeManagerPtr getStudentGradeManager();

private:
    void initAppData();
    void initAppVersionInfo();
    void initManagers();

private:
    UserInfo mLoginUserInfo;
    AppVersionInfo mVersionInfo;

    DataManagerPtr mDataManager;
    UserManagerPtr mUserManager;
    NetworkManagerPtr mNetworkManager;
    TaskManagerPtr mTaskManager;
    AccountManagerPtr mAccountManager;
    FeedbackManagerPtr mFeedbackManager;
    StudentGradeManagerPtr mStudentGradeManager;
};

#endif // COREFRAMEWORK_H
