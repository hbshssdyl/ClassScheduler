#include "FeedbackController.h"
#include "Managers/FeedbackManager.h"
#include "Utils/ControllerUtils.h"

FeedbackController::FeedbackController(CoreFrameworkPtr& coreFramework, QObject *parent)
    : mCoreFramework(coreFramework)
    , QObject{parent}
{
    initialize();
}

void FeedbackController::initialize()
{
    refreshfeedbackList();
}

void FeedbackController::refreshfeedbackList()
{
    if(auto feedbackManager = mCoreFramework->getFeedbackManager())
    {
        mFeedbackInfos = feedbackManager->getFeedbackInfos();
    }

    if(mFeedbackInfos.empty())
    {
        LOG_INFO("mFeedbackInfos is empty");
        return;
    }

    QVariantList feedbackAppList;
    QVariantList feedbackCompanyList;
    bool isSupperAdmin = mCoreFramework->getLoginUserInfo().role == UserRole::SuperAdmin;
    LOG_INFO(isSupperAdmin +" " +mCoreFramework->getLoginUserInfo().name);
    CUtils::updatefeedbackInfosList(feedbackAppList, feedbackCompanyList, mFeedbackInfos, isSupperAdmin);

    if (mFeedbackAppList != feedbackAppList)
    {
        mFeedbackAppList = std::move(feedbackAppList);
        emit feedbackAppListChanged();
    }

    if (mFeedbackCompanyList != feedbackCompanyList)
    {
        mFeedbackCompanyList = std::move(feedbackCompanyList);
        emit feedbackCompanyListChanged();
    }
}

void FeedbackController::addFeedback(QString feedbackType, QString username, bool isAnonymous, QString feedbackMessage, QString dateAndTime)
{
    if(auto feedbackManager = mCoreFramework->getFeedbackManager())
    {
        auto result = feedbackManager->addFeedback(feedbackType.toStdString(), username.toStdString(), isAnonymous, feedbackMessage.toStdString(), dateAndTime.toStdString());
        LOG_INFO("add Feedback: " +result);
        if(result)
        {
            refreshfeedbackList();
        }
    }
}

void FeedbackController::likeFeedback(int feedbackId)
{
    if(auto feedbackManager = mCoreFramework->getFeedbackManager())
    {
        auto result = feedbackManager->likeFeedback(feedbackId);
        LOG_INFO("like Feedback: " +result);
        if(result)
        {
            refreshfeedbackList();
        }
    }
}

void FeedbackController::approveFeedback(int feedbackId)
{
    if(auto feedbackManager = mCoreFramework->getFeedbackManager())
    {
        auto result = feedbackManager->approveFeedback(feedbackId);
        LOG_INFO("approve Feedback: " +result);
        if(result)
        {
            refreshfeedbackList();
        }
    }
}

void FeedbackController::rejectFeedback(int feedbackId)
{
    if(auto feedbackManager = mCoreFramework->getFeedbackManager())
    {
        auto result = feedbackManager->rejectFeedback(feedbackId);
        LOG_INFO("reject Feedback: " +result);
        if(result)
        {
            refreshfeedbackList();
        }
    }
}

void FeedbackController::deleteFeedback(int feedbackId)
{
    if(auto feedbackManager = mCoreFramework->getFeedbackManager())
    {
        auto result = feedbackManager->deleteFeedback(feedbackId);
        LOG_INFO("delete Feedback: " +result);
        if(result)
        {
            refreshfeedbackList();
        }
    }
}

