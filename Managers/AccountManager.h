#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include "Utils/DataUtils.h"

using namespace ClassScheduler;

using CoreFrameworkPtr = std::shared_ptr<class CoreFramework>;

class AccountManager : public std::enable_shared_from_this<AccountManager>
{
public:
    AccountManager(CoreFrameworkPtr coreFramework);
    UserInfos getUsers();
    void initialize();

    bool addUser(UserInfo user);

private:
    void initUsers();
    UserInfos getUsersFromServer();

private:
    std::weak_ptr<CoreFramework> mCoreFramework;
    UserInfos mUsers;
};

#endif // ACCOUNTMANAGER_H
