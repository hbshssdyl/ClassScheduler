#ifndef COREFRAMEWORK_H
#define COREFRAMEWORK_H

#include "DataManager.h"
#include "UserManager.h"
#include "NetworkManager.h"
#include "TaskManager.h"

using namespace ClassScheduler;
using CoreFrameworkPtr = std::shared_ptr<class CoreFramework>;

class CoreFramework
{
public:
    CoreFramework();
    void initialize();

private:
    DataManagerPtr mDataManager;
    UserManagerPtr mUserManager;
    NetworkManagerPtr mNetworkManager;
    TaskManagerPtr mTaskManager;
};

#endif // COREFRAMEWORK_H
