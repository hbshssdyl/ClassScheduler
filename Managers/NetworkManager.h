#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#include "Utils/DataUtils.h"

using NetworkManagerPtr = std::shared_ptr<class NetworkManager>;

class NetworkManager {
public:
    NetworkManager();
    void sendRegisterRequest(const std::string& username, const std::string& password, const std::string& role);
    void sendLoginRequest(const std::string& username, const std::string& password);
    void sendDeleteUserRequest(const std::string& username);
    void sendClearUsersRequest();
    void sendChangePasswordRequest(const std::string& username, const std::string& oldPassword, const std::string& newPassword);
    void sendUpdateRoleRequest(const std::string& username, const std::string& newRole);
};

#endif // NETWORKMANAGER_H
