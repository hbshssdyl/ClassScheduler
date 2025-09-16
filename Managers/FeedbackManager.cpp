#include "FeedbackManager.h"
#include "NetworkManager.h"
#include "CoreFramework.h"

FeedbackManager::FeedbackManager(CoreFrameworkPtr coreFramework)
    : mCoreFramework(coreFramework)
{
}

FeedbackInfos FeedbackManager::getFeedbackInfos()
{
    return mFeedbackInfos;
}

void FeedbackManager::initialize()
{
    initFeedbackInfos();
}

void FeedbackManager::initFeedbackInfos()
{
    mFeedbackInfos = getFeedbacksFromServer();
}

FeedbackInfos FeedbackManager::getFeedbacksFromServer()
{
    std::cout << "getFeedbacksFromServer " << std::endl;
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            auto response = networkManager->getAllFeedbacks();
            std::cout << response.rawResponse << std::endl;
            if(response.status == ResultStatus::GetAllFeedbacksSuccess)
            {
                return response.feedbackInfos;
            }
            return {};
        }
    }
    std::cout << "no coreFramework or no networkManager" << std::endl;
    return {};
}

bool FeedbackManager::addFeedback(std::string feedbackType, std::string username, bool isAnonymous, std::string feedbackMessage, std::string dateAndTime)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            auto loginUserInfo = coreFramework->getLoginUserInfo();
            FeedbackCreateReq requestInfo;
            requestInfo.type = toFeedbackType(feedbackType);
            requestInfo.userType = isAnonymous ? FeedbackUserType::Anonymous : FeedbackUserType::RealName;
            requestInfo.message = feedbackMessage;
            requestInfo.feedbackUsername = username;
            requestInfo.dateAndTime = dateAndTime;
            requestInfo.realUsername = loginUserInfo.name;
            requestInfo.realEmail = loginUserInfo.email;

            auto result = networkManager->createFeedback(requestInfo);
            std::cout << result.statusStr << std::endl;
            std::cout << result.rawResponse << std::endl;
            if(result.status == ResultStatus::CreateFeedbackSuccess)
            {
                initFeedbackInfos();
                return true;
            }
        }
    }
    return false;
}

bool FeedbackManager::likeFeedback(int feedbackId)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            std::cout << "FeedbackInfos size: " << mFeedbackInfos.size() << std::endl;
            for(auto& feedbackInfo : mFeedbackInfos)
            {
                std::cout << "feedbackInfos.id: " << feedbackInfo.id << ", feedbackId: " << feedbackId << ", feedbackInfos.Status: " << to_string(feedbackInfo.feedbackStatus) << std::endl;
                if(feedbackInfo.id == feedbackId && feedbackInfo.feedbackStatus == FeedbackStatus::Approved)
                {
                    auto result = networkManager->likeFeedback(feedbackId);
                    std::cout << result.statusStr << std::endl;
                    std::cout << result.rawResponse << std::endl;
                    if(result.status == ResultStatus::LikeFeedbackSuccess)
                    {
                        feedbackInfo.likeCount = feedbackInfo.likeCount + 1;
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

bool FeedbackManager::approveFeedback(int feedbackId)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            std::cout << "FeedbackInfos size: " << mFeedbackInfos.size() << std::endl;
            for(auto& feedbackInfo : mFeedbackInfos)
            {
                std::cout << "feedbackInfos.id: " << feedbackInfo.id << ", feedbackId: " << feedbackId << ", feedbackInfos.Status: " << to_string(feedbackInfo.feedbackStatus) << std::endl;
                if(feedbackInfo.id == feedbackId && feedbackInfo.feedbackStatus == FeedbackStatus::Submitted)
                {
                    FeedbackUpdateStatusReq requestInfo;
                    requestInfo.id = feedbackId;
                    requestInfo.newStatus = FeedbackStatus::Approved;
                    auto result = networkManager->updateFeedbackStatus(requestInfo);
                    std::cout << result.statusStr << std::endl;
                    std::cout << result.rawResponse << std::endl;
                    if(result.status == ResultStatus::UpdateFeedbackStatusSuccess)
                    {
                        feedbackInfo.feedbackStatus = FeedbackStatus::Approved;
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

bool FeedbackManager::rejectFeedback(int feedbackId)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            std::cout << "FeedbackInfos size: " << mFeedbackInfos.size() << std::endl;
            for(auto& feedbackInfo : mFeedbackInfos)
            {
                std::cout << "feedbackInfos.id: " << feedbackInfo.id << ", feedbackId: " << feedbackId << ", feedbackInfos.Status: " << to_string(feedbackInfo.feedbackStatus) << std::endl;
                if(feedbackInfo.id == feedbackId && feedbackInfo.feedbackStatus == FeedbackStatus::Submitted)
                {
                    FeedbackUpdateStatusReq requestInfo;
                    requestInfo.id = feedbackId;
                    requestInfo.newStatus = FeedbackStatus::Rejected;
                    auto result = networkManager->updateFeedbackStatus(requestInfo);
                    std::cout << result.statusStr << std::endl;
                    std::cout << result.rawResponse << std::endl;
                    if(result.status == ResultStatus::UpdateFeedbackStatusSuccess)
                    {
                        feedbackInfo.feedbackStatus = FeedbackStatus::Rejected;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool FeedbackManager::deleteFeedback(int feedbackId)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            std::cout << "FeedbackInfos size: " << mFeedbackInfos.size() << std::endl;
            for(auto& feedbackInfo : mFeedbackInfos)
            {
                std::cout << "feedbackInfos.id: " << feedbackInfo.id << ", feedbackId: " << feedbackId << ", feedbackInfos.Status: " << to_string(feedbackInfo.feedbackStatus) << std::endl;
                if(feedbackInfo.id == feedbackId)
                {
                    auto result = networkManager->deleteFeedback(feedbackId);
                    std::cout << result.statusStr << std::endl;
                    std::cout << result.rawResponse << std::endl;
                    if(result.status == ResultStatus::DeleteFeedbackSuccess)
                    {
                        initFeedbackInfos();
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
