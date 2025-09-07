#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#include "Utils/DataUtils.h"

using namespace ClassScheduler;

using CoreFrameworkPtr = std::shared_ptr<class CoreFramework>;

class NetworkManager : public std::enable_shared_from_this<NetworkManager>
{
public:
    NetworkManager(CoreFrameworkPtr coreFramework);
    ResponseResult sendRegisterRequest(const std::string& email, const std::string& username, const std::string& password, const std::string& role);
    ResponseResult approveUserRequest(int userId);
    ResponseResult rejectUserRequest(int userId);
    ResponseResult blacklistUserRequest(int userId);
    ResponseResult deleteUserRequest(int userId);
    ResponseResult addUserRequest(const std::string& username, const std::string& password,
                                  const std::string& email, const std::string& role);
    ResponseResult getAllUsersRequest();
    ResponseResult sendLoginRequest(const std::string& login, const std::string& password);
    ResponseResult sendDeleteUserRequest(const std::string& username);
    ResponseResult sendClearUsersRequest();
    ResponseResult sendChangePasswordRequest(const std::string& username, const std::string& oldPassword, const std::string& newPassword);
    ResponseResult sendUpdateRoleRequest(const std::string& username, const std::string& newRole);
    ResponseResult uploadDbFile();
    ResponseResult downloadDbFile();
    ResponseResult createOneToOneTask(const Task& task);
    ResponseResult updateOneToOneTask(int taskId, const Task& task);
    ResponseResult deleteOneToOneTask(int taskId);
    ResponseResult getAllOneToOneTasks();
    ResponseResult getOneToOneTaskById(int taskId);

    // Feedback APIs
    ResponseResult createFeedback(const FeedbackCreateReq& req);
    ResponseResult updateFeedbackStatus(const FeedbackUpdateStatusReq& req);
    ResponseResult deleteFeedback(int feedbackId);
    ResponseResult getFeedbackById(int feedbackId);
    ResponseResult getAllFeedbacks();
    ResponseResult likeFeedback(int feedbackId);

private:
    std::weak_ptr<CoreFramework> mCoreFramework;
};

#endif // NETWORKMANAGER_H
