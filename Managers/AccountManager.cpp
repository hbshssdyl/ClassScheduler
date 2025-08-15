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
            std::cout << response.rawResponse << std::endl;
            if(response.status == ResultStatus::GetAllUsersSuccess)
            {
                return response.userInfos;
            }
            return {};
        }
    }
    std::cout << "no coreFramework or no networkManager" << std::endl;
    return {};
}

bool AccountManager::approveUser(int userId)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            std::cout << "Users size: " << mUsers.size() << std::endl;
            for(auto& user : mUsers)
            {
                std::cout << "user.id: " << user.id << ", userId: " << userId << ", user.accountStatus: " << user.accountStatus << std::endl;
                if(user.id == userId && user.accountStatus == toAccountStatus(AccountStatus::REGISTERED))
                {
                    user.accountStatus = toAccountStatus(AccountStatus::APPROVED);
                    auto result = networkManager->approveUserRequest(userId);
                    std::cout << result.statusStr << std::endl;
                    std::cout << result.rawResponse << std::endl;
                    if(result.status == ResultStatus::ApproveUserSuccess)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool AccountManager::rejectUser(int userId)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            std::cout << "Users size: " << mUsers.size() << std::endl;
            for(auto& user : mUsers)
            {
                std::cout << "user.id: " << user.id << ", userId: " << userId << ", user.accountStatus: " << user.accountStatus << std::endl;
                if(user.id == userId && user.accountStatus == toAccountStatus(AccountStatus::REGISTERED))
                {
                    user.accountStatus = toAccountStatus(AccountStatus::REJECTED);
                    auto result = networkManager->rejectUserRequest(userId);
                    std::cout << result.statusStr << std::endl;
                    std::cout << result.rawResponse << std::endl;
                    if(result.status == ResultStatus::RejectUserSuccess)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool AccountManager::blacklistUser(int userId)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            std::cout << "Users size: " << mUsers.size() << std::endl;
            for(auto& user : mUsers)
            {
                std::cout << "user.id: " << user.id << ", userId: " << userId << ", user.accountStatus: " << user.accountStatus << std::endl;
                if(user.id == userId && user.accountStatus == toAccountStatus(AccountStatus::APPROVED))
                {
                    user.accountStatus = toAccountStatus(AccountStatus::BLACKLISTED);
                    auto result = networkManager->blacklistUserRequest(userId);
                    std::cout << result.statusStr << std::endl;
                    std::cout << result.rawResponse << std::endl;
                    if(result.status == ResultStatus::BlacklistUserSuccess)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool AccountManager::deleteUser(int userId)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            std::cout << "Users size: " << mUsers.size() << std::endl;
            for(auto& user : mUsers)
            {
                std::cout << "user.id: " << user.id << ", userId: " << userId << ", user.accountStatus: " << user.accountStatus << std::endl;
                if(user.id == userId && (user.accountStatus == toAccountStatus(AccountStatus::APPROVED) && user.accountStatus == toAccountStatus(AccountStatus::BLACKLISTED)))
                {
                    auto result = networkManager->deleteUserRequest(userId);
                    std::cout << result.statusStr << std::endl;
                    std::cout << result.rawResponse << std::endl;
                    if(result.status == ResultStatus::DeleteUserSuccess)
                    {
                        initUsers();
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool AccountManager::addUser(std::string username, std::string password, std::string email)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            std::cout << "Users size: " << mUsers.size() << std::endl;
            for(auto& user : mUsers)
            {
                std::cout << "user.id: " << user.id << ", userId: " << userId << ", user.accountStatus: " << user.accountStatus << std::endl;
                if(user.id == userId && (user.accountStatus == toAccountStatus(AccountStatus::APPROVED) && user.accountStatus == toAccountStatus(AccountStatus::BLACKLISTED)))
                {
                    auto result = networkManager->deleteUserRequest(userId);
                    std::cout << result.statusStr << std::endl;
                    std::cout << result.rawResponse << std::endl;
                    if(result.status == ResultStatus::ApproveUserSuccess)
                    {
                        initUsers();
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
