#include "AccountManager.h"
#include "NetworkManager.h"
#include "CoreFramework.h"
#include <iostream>

AccountManager::AccountManager(CoreFrameworkPtr coreFramework)
    : mCoreFramework(coreFramework)
{
}

void AccountManager::initialize()
{
    initUsers();
}

void AccountManager::initUsers()
{
    mUsers = getUsersFromServer();
}

UserInfos AccountManager::getUsers()
{
    return mUsers;
}

UserInfos AccountManager::getUsersFromServer()
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            auto response = networkManager->getAllUsersRequest();
            if(response.status == ResultStatus::GetAllUsersSuccess)
            {
                return response.userInfos;
            }
            std::cout << response.rawResponse << std::endl;
            return {};
        }
    }
    std::cout << "no coreFramework or no networkManager" << std::endl;
    return {};
}
