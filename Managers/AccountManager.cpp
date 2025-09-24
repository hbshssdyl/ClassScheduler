#include "AccountManager.h"
#include "NetworkManager.h"
#include "CoreFramework.h"

AccountManager::AccountManager(CoreFrameworkPtr coreFramework)
    : mCoreFramework(coreFramework)
{
}

void AccountManager::initialize()
{
    initUsers();
}

void AccountManager::refreshDataFromServer()
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
            LOG_INFO(response.rawResponse);
            if(response.status == ResultStatus::GetAllUsersSuccess)
            {
                return response.userInfos;
            }
            return {};
        }
    }
    LOG_INFO("no coreFramework or no networkManager");
    return {};
}

bool AccountManager::approveUser(int userId)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            LOG_INFO("Users size: " + std::to_string(mUsers.size()));
            for(auto& user : mUsers)
            {
                LOG_INFO("user.id: " + std::to_string(user.id) +", userId: " + std::to_string(userId) +", user.accountStatus: " +user.accountStatus);
                if(user.id == userId && user.accountStatus == toAccountStatus(AccountStatus::REGISTERED))
                {
                    user.accountStatus = toAccountStatus(AccountStatus::APPROVED);
                    auto result = networkManager->approveUserRequest(userId);
                    LOG_INFO(result.statusStr);
                    LOG_INFO(result.rawResponse);
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
            LOG_INFO("Users size: " +std::to_string(mUsers.size()));
            for(auto& user : mUsers)
            {
                LOG_INFO("user.id: " + std::to_string(user.id) +", userId: " + std::to_string(userId) +", user.accountStatus: " +user.accountStatus);
                if(user.id == userId && user.accountStatus == toAccountStatus(AccountStatus::REGISTERED))
                {
                    user.accountStatus = toAccountStatus(AccountStatus::REJECTED);
                    auto result = networkManager->rejectUserRequest(userId);
                    LOG_INFO(result.statusStr);
                    LOG_INFO(result.rawResponse);
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
            LOG_INFO("Users size: " +std::to_string(mUsers.size()));
            for(auto& user : mUsers)
            {
                LOG_INFO("user.id: " + std::to_string(user.id) +", userId: " + std::to_string(userId) +", user.accountStatus: " +user.accountStatus);
                if(user.id == userId && user.accountStatus == toAccountStatus(AccountStatus::APPROVED))
                {
                    user.accountStatus = toAccountStatus(AccountStatus::BLACKLISTED);
                    auto result = networkManager->blacklistUserRequest(userId);
                    LOG_INFO(result.statusStr);
                    LOG_INFO(result.rawResponse);
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
            LOG_INFO("Users size: " +std::to_string(mUsers.size()));
            for(auto& user : mUsers)
            {
                LOG_INFO("user.id: " + std::to_string(user.id) +", userId: " + std::to_string(userId) +", user.accountStatus: " +user.accountStatus);
                if(user.id == userId)
                {
                    auto result = networkManager->deleteUserRequest(userId);
                    LOG_INFO(result.statusStr);
                    LOG_INFO(result.rawResponse);
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

bool AccountManager::addUser(std::string username, std::string password, std::string email, std::string role)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            auto result = networkManager->addUserRequest(username, password, email, role);
            LOG_INFO(result.statusStr);
            LOG_INFO(result.rawResponse);
            if(result.status == ResultStatus::AddUserSuccess)
            {
                initUsers();
                return true;
            }
        }
    }
    return false;
}
