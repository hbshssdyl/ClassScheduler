#ifndef USERMANAGER_H
#define USERMANAGER_H
#include "Utils/DataUtils.h"

using namespace ClassScheduler;

using UserManagerPtr = std::shared_ptr<class UserManager>;

class UserManager
{
public:
    UserManager();
    UserInfo getUserInfoByLoginInfo(std::string& username, std::string& password, LoginInfos& loginInfos);

private:
    OperateModes getModsByUserLevel(UserLevel& level);
    std::string getNameByUsername(std::string username);
};

#endif // USERMANAGER_H
