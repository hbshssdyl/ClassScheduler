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
        cout << "mFeedbackInfos is empty" << endl;
        return;
    }

    QVariantList feedbackAppList;
    QVariantList feedbackCompanyList;
    bool isSupperAdmin = mCoreFramework->getLoginUserInfo().role == UserRole::SuperAdmin;

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
        cout << "add Feedback: " << result << endl;
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
        cout << "like Feedback: " << result << endl;
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
        cout << "approve Feedback: " << result << endl;
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
        cout << "reject Feedback: " << result << endl;
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
        cout << "delete Feedback: " << result << endl;
        if(result)
        {
            refreshfeedbackList();
        }
    }
}

