#include "NetworkManager.h"
#include "CoreFramework.h"
#include <string>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static const std::string SERVER_URL = "http://127.0.0.1:8888/";

NetworkManager::NetworkManager(CoreFrameworkPtr coreFramework)
    : mCoreFramework(coreFramework)
{
}

size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

size_t writeFileCallback(void* ptr, size_t size, size_t nmemb, void* stream) {
    FILE* f = (FILE*)stream;
    return fwrite(ptr, size, nmemb, f);
}

std::function<void(int)> downloadProgressCallback;
int progressCallback(void* ptr, curl_off_t totalToDownload, curl_off_t nowDownloaded, curl_off_t totalToUpload, curl_off_t nowUploaded) {
    if (totalToDownload > 0 && downloadProgressCallback) {
        int progress = static_cast<int>((nowDownloaded * 100) / totalToDownload);
        downloadProgressCallback(progress);
    }
    return 0; // 返回非0可中断下载
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
    payload["role"] = role;  // 例如 "员工" 或 "高级员工"

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

// PUT /admin/users/{user_id}/approve
ResponseResult NetworkManager::approveUserRequest(int userId) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    curl = curl_easy_init();
    if (curl) {
        std::string url = SERVER_URL + "admin/users/" + std::to_string(userId) + "/approve";
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, (std::string("X-Username: ") + ADMIN_USER_NAME).c_str());

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            result.rawResponse = curl_easy_strerror(res);
        else
            result.refreshResult(responseStr);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return result;
}

// 其他 PUT 类接口（reject / blacklist）
ResponseResult NetworkManager::rejectUserRequest(int userId) {
    // 和 approveUserRequest 基本一致，只是 URL 改为 "/reject"
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    curl = curl_easy_init();
    if (curl) {
        std::string url = SERVER_URL + "admin/users/" + std::to_string(userId) + "/reject";
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, (std::string("X-Username: ") + ADMIN_USER_NAME).c_str());

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            result.rawResponse = curl_easy_strerror(res);
        else
            result.refreshResult(responseStr);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return result;
}

ResponseResult NetworkManager::blacklistUserRequest(int userId) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    curl = curl_easy_init();
    if (curl) {
        std::string url = SERVER_URL + "admin/users/" + std::to_string(userId) + "/blacklist";
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, (std::string("X-Username: ") + ADMIN_USER_NAME).c_str());

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            result.rawResponse = curl_easy_strerror(res);
        else
            result.refreshResult(responseStr);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return result;
}

// DELETE /admin/users/{user_id}
ResponseResult NetworkManager::deleteUserRequest(int userId) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    curl = curl_easy_init();
    if (curl) {
        std::string url = SERVER_URL + "admin/users/" + std::to_string(userId);
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, (std::string("X-Username: ") + ADMIN_USER_NAME).c_str());

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            result.rawResponse = curl_easy_strerror(res);
        else
            result.refreshResult(responseStr);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return result;
}

// POST /admin/users
ResponseResult NetworkManager::addUserRequest(const std::string& username, const std::string& password,
                                              const std::string& email, const std::string& role) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    json payload;
    payload["email"] = email;
    payload["username"] = username;
    payload["password"] = password;
    payload["role"] = role;

    std::string jsonData = payload.dump();

    curl = curl_easy_init();
    if (curl) {
        std::string url = SERVER_URL + "admin/users";
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, (std::string("X-Username: ") + ADMIN_USER_NAME).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, jsonData.size());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            result.rawResponse = curl_easy_strerror(res);
        else
            result.refreshResult(responseStr);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return result;
}

// GET /admin/users
ResponseResult NetworkManager::getAllUsersRequest() {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    curl = curl_easy_init();
    if (curl) {
        std::string url = SERVER_URL + "admin/users";
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, (std::string("X-Username: ") + ADMIN_USER_NAME).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            result.rawResponse = curl_easy_strerror(res);
        else
            result.refreshResult(responseStr);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
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
        curl_mime_filedata(field, databaseFullPath().toStdString().c_str());
        curl_mime_filename(field, DATABASE_NAME);

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

    FILE* fp = fopen(databaseFullPath().toStdString().c_str(), "wb");
    if (!fp) {
        result.status = ResultStatus::CreateDatabaseFileFailed;
        result.rawResponse = "无法创建本地文件: " + databaseFullPath().toStdString();
        result.statusStr = result.toString(ResultStatus::CreateDatabaseFileFailed);
        return result;
    }
    LOG_INFO("test: " +databaseFullPath().toStdString());

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

ResponseResult NetworkManager::createOneToOneTask(const Task& task) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    json payload = {
        {"title", task.title},
        {"category", task.category},
        {"description", task.description},
        {"publish", task.publish},
        {"due", task.due},
        {"rating", task.rating},
        {"finishStatus", task.finishStatus},
        {"comment", task.comment},
        {"reviewString", task.reviewString},
        {"resultRating", task.resultRating},
        {"reviewStatus", task.reviewStatus}
    };

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        std::string jsonData = payload.dump();

        curl_easy_setopt(curl, CURLOPT_URL, (SERVER_URL + "tasks/one-to-one/add").c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            result.rawResponse = curl_easy_strerror(res);
        else
            result.refreshResult(responseStr);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return result;
}

ResponseResult NetworkManager::updateOneToOneTask(int taskId, const Task& task) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    json payload = {
        {"title", task.title},
        {"category", task.category},
        {"description", task.description},
        {"publish", task.publish},
        {"due", task.due},
        {"rating", task.rating},
        {"finishStatus", task.finishStatus},
        {"comment", task.comment},
        {"reviewString", task.reviewString},
        {"resultRating", task.resultRating},
        {"reviewStatus", task.reviewStatus}
    };

    std::string url = SERVER_URL + "tasks/one-to-one/update/" + std::to_string(taskId);

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        std::string jsonData = payload.dump();

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            result.rawResponse = curl_easy_strerror(res);
        else
            result.refreshResult(responseStr);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return result;
}

ResponseResult NetworkManager::deleteOneToOneTask(int taskId) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    std::string url = SERVER_URL + "tasks/one-to-one/delete" + std::to_string(taskId);

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            result.rawResponse = curl_easy_strerror(res);
        else
            result.refreshResult(responseStr);

        curl_easy_cleanup(curl);
    }

    return result;
}

ResponseResult NetworkManager::getAllOneToOneTasks() {
    CURL* curl = curl_easy_init();
    std::string responseStr;
    ResponseResult result;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, (SERVER_URL + "tasks/one-to-one/all").c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            result.rawResponse = curl_easy_strerror(res);
        } else {
            LOG_INFO(responseStr);
            result.refreshResult(responseStr);
        }

        curl_easy_cleanup(curl);
    }

    return result;
}

ResponseResult NetworkManager::getOneToOneTaskById(int taskId) {
    CURL* curl;
    CURLcode res;
    std::string responseStr;
    ResponseResult result;

    std::string url = SERVER_URL + "tasks/one-to-one/" + std::to_string(taskId);

    curl = curl_easy_init();
    if (curl) {
        // 配置GET请求（默认方法，无需设置CUSTOMREQUEST）
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        // 执行请求
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            result.rawResponse = curl_easy_strerror(res);
        } else {
            LOG_INFO("Response: " +responseStr);
            result.refreshResult(responseStr);
        }

        curl_easy_cleanup(curl);
    }

    return result;
}

// 小工具：URL 编码（避免中文枚举在查询字符串里乱码）
static std::string urlEncode(CURL* curl, const std::string& s) {
    char* out = curl_easy_escape(curl, s.c_str(), static_cast<int>(s.size()));
    std::string encoded = out ? out : "";
    if (out) curl_free(out);
    return encoded;
}

// 1) 新增反馈  POST /feedbacks/add
ResponseResult NetworkManager::createFeedback(const FeedbackCreateReq& req) {
    CURL* curl = curl_easy_init();
    std::string responseStr;
    ResponseResult result;

    if (!curl) { result.updateToCurlError(); return result; }

    json payload = {
        {"feedbackType",   to_string(req.type)},
        {"userType",       to_string(req.userType)},
        {"feedbackMessage",req.message},
        {"dateAndTime",    req.dateAndTime}
    };

    LOG_INFO(req.dateAndTime.value());

    if (req.feedbackUsername && !req.feedbackUsername->empty())
        payload["feedbackUsername"] = *req.feedbackUsername;
    if (req.realUsername && !req.realUsername->empty())
        payload["realUsername"] = *req.realUsername;
    if (req.realEmail && !req.realEmail->empty())
        payload["realEmail"] = *req.realEmail;

    std::string jsonData = payload.dump();

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, (SERVER_URL + "feedbacks/add").c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) result.rawResponse = curl_easy_strerror(res);
    else                 result.refreshResult(responseStr);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return result;
}

// 2) 更新状态  PUT /feedbacks/update-status
//   （后端若使用 /update-status/{id} 也行，把 URL 换一下并去掉 payload.id）
ResponseResult NetworkManager::updateFeedbackStatus(const FeedbackUpdateStatusReq& req) {
    CURL* curl = curl_easy_init();
    std::string responseStr;
    ResponseResult result;

    if (!curl) { result.updateToCurlError(); return result; }

    json payload = {
        {"id", req.id},
        {"newStatus", to_string(req.newStatus)}
    };
    std::string jsonData = payload.dump();

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_URL, (SERVER_URL + "feedbacks/update-status").c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) result.rawResponse = curl_easy_strerror(res);
    else                 result.refreshResult(responseStr);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return result;
}

// 3) 删除反馈  POST /feedbacks/delete  { "id": x }
ResponseResult NetworkManager::deleteFeedback(int feedbackId) {
    CURL* curl = curl_easy_init();
    std::string responseStr;
    ResponseResult result;

    if (!curl) { result.updateToCurlError(); return result; }

    json payload = { {"id", feedbackId} };
    std::string jsonData = payload.dump();

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, (SERVER_URL + "feedbacks/delete").c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) result.rawResponse = curl_easy_strerror(res);
    else                 result.refreshResult(responseStr);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return result;
}

// 4) 获取所有反馈
ResponseResult NetworkManager::getAllFeedbacks() {
    ResponseResult result;
    CURL* curl = curl_easy_init();
    if (!curl) {
        result.updateToCurlError();
        return result;
    }

    std::string responseStr;
    curl_easy_setopt(curl, CURLOPT_URL, (SERVER_URL + "feedbacks/all").c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        result.rawResponse = curl_easy_strerror(res);
    else
        result.refreshResult(responseStr);

    curl_easy_cleanup(curl);
    return result;
}

// 5) 获取单条  GET /feedbacks/{id}
ResponseResult NetworkManager::getFeedbackById(int feedbackId) {
    CURL* curl = curl_easy_init();
    std::string responseStr;
    ResponseResult result;

    if (!curl) { result.updateToCurlError(); return result; }

    std::string url = SERVER_URL + "feedbacks/" + std::to_string(feedbackId);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) result.rawResponse = curl_easy_strerror(res);
    else                 result.refreshResult(responseStr);

    curl_easy_cleanup(curl);
    return result;
}

// 6) 点赞  POST /feedbacks/like/{id}
ResponseResult NetworkManager::likeFeedback(int feedbackId) {
    CURL* curl = curl_easy_init();
    std::string responseStr;
    ResponseResult result;

    if (!curl) { result.updateToCurlError(); return result; }

    std::string url = SERVER_URL + "feedbacks/like/" + std::to_string(feedbackId);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) result.rawResponse = curl_easy_strerror(res);
    else                 result.refreshResult(responseStr);

    curl_easy_cleanup(curl);
    return result;
}

ResponseResult NetworkManager::getLatestVersion() {
    CURL* curl = curl_easy_init();
    ResponseResult result;
    if(curl) {
        std::string responseStr;
        curl_easy_setopt(curl, CURLOPT_URL, (SERVER_URL + "latest-version").c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            result.rawResponse = curl_easy_strerror(res);
        else
            result.refreshResult(responseStr);

        curl_easy_cleanup(curl);
    } else {
        result.updateToCurlError();
    }
    return result;
}

void NetworkManager::downloadInstaller(const std::string& url, const std::string& savePath, std::function<void(int)> callback) {
    CURL* curl = curl_easy_init();
    if(!curl) return;

    FILE* fp = fopen(savePath.c_str(), "wb");
    if(!fp) {
        curl_easy_cleanup(curl);
        LOG_INFO("Failed to open savePath, savePath: " + savePath);
        return;
    }

    downloadProgressCallback = std::move(callback);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFileCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressCallback);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);

    CURLcode res = curl_easy_perform(curl);
    fclose(fp);
    curl_easy_cleanup(curl);

    return;
}

