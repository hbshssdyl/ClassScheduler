#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#include "Utils/DataUtils.h"

using namespace ClassScheduler;

using NetworkManagerPtr = std::shared_ptr<class NetworkManager>;

class NetworkManager
{
public:
    NetworkManager();
    void sendLoginRequest(const std::string& username, const std::string& password);
};

#endif // NETWORKMANAGER_H
