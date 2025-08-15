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

    bool approveUser(int userId);
    bool rejectUser(int userId);
    bool blacklistUser(int userId);
    bool deleteUser(int userId);
    bool addUser(std::string username, std::string password, std::string email);

private:
    void initUsers();
    UserInfos getUsersFromServer();

private:
    std::weak_ptr<CoreFramework> mCoreFramework;
    UserInfos mUsers;
};

#endif // ACCOUNTMANAGER_H
