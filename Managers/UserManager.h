#ifndef USERMANAGER_H
#define USERMANAGER_H
#include "Utils/DataUtils.h"

using namespace ClassScheduler;

using UserManagerPtr = std::shared_ptr<class UserManager>;

class UserManager
{
public:
    UserManager();
    UserInfo getUserInfoByLoginInfo(std::string& username, UserRole& role);

private:
    OperateModes getModsByUserRole(UserRole& role);
};

#endif // USERMANAGER_H
