#ifndef COREFRAMEWORK_H
#define COREFRAMEWORK_H

#include "Utils/DataUtils.h"

using namespace ClassScheduler;
using CoreFrameworkPtr = std::shared_ptr<class CoreFramework>;
using DataManagerPtr = std::shared_ptr<class DataManager>;
using UserManagerPtr = std::shared_ptr<class UserManager>;
using NetworkManagerPtr = std::shared_ptr<class NetworkManager>;
using TaskManagerPtr = std::shared_ptr<class TaskManager>;

class CoreFramework : public std::enable_shared_from_this<CoreFramework>
{
public:
    CoreFramework();
    void initialize();
    DataManagerPtr getDataManager();
    UserManagerPtr getUserManager();
    NetworkManagerPtr getNetworkManager();
    TaskManagerPtr getTaskManager();

private:
    void initAppData();
    void initManagers();

private:
    DataManagerPtr mDataManager;
    UserManagerPtr mUserManager;
    NetworkManagerPtr mNetworkManager;
    TaskManagerPtr mTaskManager;
};

#endif // COREFRAMEWORK_H
