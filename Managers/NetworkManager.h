#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#include "Utils/DataUtils.h"

using namespace ClassScheduler;
using NetworkManagerPtr = std::shared_ptr<class NetworkManager>;

class NetworkManager {
public:
    NetworkManager();
    ResponseResult sendRegisterRequest(const std::string& email, const std::string& username, const std::string& password, const std::string& role);
    ResponseResult sendLoginRequest(const std::string& login, const std::string& password);
    ResponseResult sendDeleteUserRequest(const std::string& username);
    ResponseResult sendClearUsersRequest();
    ResponseResult sendChangePasswordRequest(const std::string& username, const std::string& oldPassword, const std::string& newPassword);
    ResponseResult sendUpdateRoleRequest(const std::string& username, const std::string& newRole);
};

#endif // NETWORKMANAGER_H
