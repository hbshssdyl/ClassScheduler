﻿#include "NetworkManager.h"
#include <string>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static const std::string SERVER_URL = "http://127.0.0.1:8888/";

NetworkManager::NetworkManager() {}

size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

ResponseResult NetworkManager::sendRegisterRequest(const std::string& email, const std::string& username, const std::string& password, const std::string& role) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    // 构造 JSON 数据
    json payload;
    payload["email"] = email;
    payload["username"] = username;
    payload["password"] = password;
    payload["role"] = role;  // 例如 "普通员工" 或 "高级员工"

    std::string jsonData = payload.dump();

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, (SERVER_URL + "register").c_str());  // 注意端口号和接口一致
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, jsonData.size());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            result.rawResponse = curl_easy_strerror(res);
        } else {
            result.refreshResult(responseStr);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        result.updateToCurlError();
    }
    return result;
}

ResponseResult NetworkManager::sendLoginRequest(const std::string& login, const std::string& password) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    json payload;
    payload["login"] = login;
    payload["password"] = password;

    std::string jsonData = payload.dump();

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, (SERVER_URL + "login").c_str()); // ✅ 改成登录接口
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            result.rawResponse = curl_easy_strerror(res);
        } else {
            result.refreshResult(responseStr);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        result.updateToCurlError();
    }
    return result;
}

ResponseResult NetworkManager::sendDeleteUserRequest(const std::string& username) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    json payload;
    payload["username"] = username;
    std::string jsonData = payload.dump();

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_URL, (SERVER_URL + "delete").c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            result.rawResponse = curl_easy_strerror(res);
        } else {
            result.refreshResult(responseStr);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        result.updateToCurlError();
    }
    return result;
}

ResponseResult NetworkManager::sendClearUsersRequest() {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_URL, (SERVER_URL + "clear").c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            result.rawResponse = curl_easy_strerror(res);
        } else {
            result.refreshResult(responseStr);
        }

        curl_easy_cleanup(curl);
    } else {
        result.updateToCurlError();
    }
    return result;
}

ResponseResult NetworkManager::sendChangePasswordRequest(const std::string& username, const std::string& oldPassword, const std::string& newPassword) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

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
        curl_easy_setopt(curl, CURLOPT_URL, (SERVER_URL + "change-password").c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            result.rawResponse = curl_easy_strerror(res);
        } else {
            result.refreshResult(responseStr);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        result.updateToCurlError();
    }
    return result;
}

ResponseResult NetworkManager::sendUpdateRoleRequest(const std::string& username, const std::string& newRole) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    json payload;
    payload["username"] = username;
    payload["new_role"] = newRole;

    std::string jsonData = payload.dump();

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_URL, (SERVER_URL + "update-role").c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            result.rawResponse = curl_easy_strerror(res);
        } else {
            result.refreshResult(responseStr);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        result.updateToCurlError();
    }
    return result;
}

ResponseResult NetworkManager::uploadDbFile() {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    curl = curl_easy_init();
    if (curl) {
        curl_mime* form = curl_mime_init(curl);
        curl_mimepart* field = curl_mime_addpart(form);
        curl_mime_name(field, "file");
        curl_mime_filedata(field, DATABASE_FULL_PATH.toStdString().c_str());
        curl_mime_filename(field, DATABASE_NAME.toStdString().c_str());

        curl_easy_setopt(curl, CURLOPT_URL, (SERVER_URL + "upload-db/").c_str());
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            result.rawResponse = curl_easy_strerror(res);
        } else {
            result.refreshResult(responseStr);
        }

        curl_mime_free(form);
        curl_easy_cleanup(curl);
    } else {
        result.updateToCurlError();
    }
    return result;
}

ResponseResult NetworkManager::downloadDbFile() {
    CURL* curl;
    CURLcode res;
    ResponseResult result;

    FILE* fp = fopen(DATABASE_FULL_PATH.toStdString().c_str(), "wb");
    if (!fp) {
        result.status = ResultStatus::CreateDatabaseFileFailed;
        result.rawResponse = "无法创建本地文件: " + DATABASE_FULL_PATH.toStdString();
        result.statusStr = result.toString(ResultStatus::CreateDatabaseFileFailed);
        return result;
    }
    std::cout << "test: " << DATABASE_FULL_PATH.toStdString() << std::endl;

    curl = curl_easy_init();
    if (curl) {
        std::string url = SERVER_URL + "export-sqlite-db/";

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nullptr);  // 使用默认写文件方式
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            result.status = ResultStatus::CurlNotOK;
            result.rawResponse = "下载失败: " + std::string(curl_easy_strerror(res));
            result.statusStr = result.toString(ResultStatus::CurlNotOK);

        } else {
            long responseCode;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
            if (responseCode == 200) {
                result.status = ResultStatus::DatabaseFileDownloadSucess;
                result.rawResponse = "数据库文件下载成功！";
                result.statusStr = result.toString(ResultStatus::DatabaseFileDownloadSucess);
            } else {
                result.status = ResultStatus::DatabaseFileDownloadFailed;
                result.rawResponse = "HTTP错误: " + std::to_string(responseCode);
                result.statusStr = result.toString(ResultStatus::DatabaseFileDownloadFailed);
            }
        }

        curl_easy_cleanup(curl);
    } else {
        result.updateToCurlError();
    }

    fclose(fp);
    return result;
}

