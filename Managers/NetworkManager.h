#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class NetworkManager {
public:
    NetworkManager();
    void sendRegisterRequest(const std::string& username, const std::string& password, const std::string& role);
    void sendLoginRequest(const std::string& username, const std::string& password);
    void sendDeleteUserRequest(const std::string& username);
    void sendClearUsersRequest();
    void sendChangePasswordRequest(const std::string& username, const std::string& oldPassword, const std::string& newPassword);
    void sendUpdateRoleRequest(const std::string& username, const std::string& newRole);

private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output);
};

#endif // NETWORKMANAGER_H
