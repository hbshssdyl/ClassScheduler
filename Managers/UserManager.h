#ifndef USERMANAGER_H
#define USERMANAGER_H
#include "Utils/DataUtils.h"

using namespace ClassScheduler;

using CoreFrameworkPtr = std::shared_ptr<class CoreFramework>;

class UserManager : public std::enable_shared_from_this<UserManager>
{
public:
    UserManager(CoreFrameworkPtr coreFramework);
    UserInfo getUserInfoByLoginInfo(std::string& username, UserRole& role);

private:
    OperateModes getModsByUserRole(UserRole& role);

private:
    std::weak_ptr<CoreFramework> mCoreFramework;
};

#endif // USERMANAGER_H
