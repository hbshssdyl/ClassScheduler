#ifndef USERMANAGER_H
#define USERMANAGER_H
#include "Utils/DataUtils.h"

using namespace ClassScheduler;

class UserManager
{
public:
    UserManager();
    UserInfo getUserInfoByLoginInfo(std::string& username, std::string& password);

private:
    OperateModes getModsByUserLevel(UserLevel& level);
};

#endif // USERMANAGER_H
