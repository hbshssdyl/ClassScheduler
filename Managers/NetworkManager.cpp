#include "NetworkManager.h"

NetworkManager::NetworkManager() {}

size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void NetworkManager::sendRegisterRequest(const std::string& username, const std::string& password, const std::string& role) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;

    // 构造 JSON 数据
    json payload;
    payload["username"] = username;
    payload["password"] = password;
    payload["role"] = role;  // 例如 "普通员工" 或 "高级员工"

    std::string jsonData = payload.dump();

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/register");  // 注意端口号和接口一致
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, jsonData.size());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "🚫 注册请求失败: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "✅ 注册接口响应: " << responseStr << std::endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "❌ 初始化 curl 失败" << std::endl;
    }
}

void NetworkManager::sendLoginRequest(const std::string& username, const std::string& password) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;

    json payload;
    payload["username"] = username;
    payload["password"] = password;

    std::string jsonData = payload.dump();

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/login"); // ✅ 改成登录接口
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "Login failed: " << curl_easy_strerror(res) << std::endl;
        else
            std::cout << "Login response: " << responseStr << std::endl;

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

void NetworkManager::sendDeleteUserRequest(const std::string& username) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;

    json payload;
    payload["username"] = username;
    std::string jsonData = payload.dump();

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/delete");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "Delete failed: " << curl_easy_strerror(res) << std::endl;
        else
            std::cout << "Delete response: " << responseStr << std::endl;

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

void NetworkManager::sendClearUsersRequest() {
    CURL* curl;
    CURLcode res;
    std::string responseStr;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/clear");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "Clear failed: " << curl_easy_strerror(res) << std::endl;
        else
            std::cout << "Clear response: " << responseStr << std::endl;

        curl_easy_cleanup(curl);
    }
}

void NetworkManager::sendChangePasswordRequest(const std::string& username, const std::string& oldPassword, const std::string& newPassword) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;

    json payload;
    payload["username"] = username;
    payload["old_password"] = oldPassword;
    payload["new_password"] = newPassword;

    std::string jsonData = payload.dump();

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/change-password");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "Change password failed: " << curl_easy_strerror(res) << std::endl;
        else
            std::cout << "Change password response: " << responseStr << std::endl;

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

void NetworkManager::sendUpdateRoleRequest(const std::string& username, const std::string& newRole) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;

    json payload;
    payload["username"] = username;
    payload["new_role"] = newRole;

    std::string jsonData = payload.dump();

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/update-role");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "Update role failed: " << curl_easy_strerror(res) << std::endl;
        else
            std::cout << "Update role response: " << responseStr << std::endl;

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

