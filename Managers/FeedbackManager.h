#ifndef FEEDBACKMANAGER_H
#define FEEDBACKMANAGER_H

#include "Utils/DataUtils.h"

using namespace ClassScheduler;

using CoreFrameworkPtr = std::shared_ptr<class CoreFramework>;

class FeedbackManager
{
public:
    FeedbackManager(CoreFrameworkPtr coreFramework);
    FeedbackInfos getFeedbackInfos();
    void initialize();

    bool addFeedback(std::string feedbackType, std::string username, bool isAnonymous, std::string feedbackMessage, std::string dateAndTime);
    bool likeFeedback(int feedbackId);
    bool approveFeedback(int feedbackId);
    bool rejectFeedback(int feedbackId);
    bool deleteFeedback(int feedbackId);

private:
    void initFeedbackInfos();
    FeedbackInfos getFeedbacksFromServer();

private:
    std::weak_ptr<CoreFramework> mCoreFramework;
    FeedbackInfos mFeedbackInfos;
};

#endif // FEEDBACKMANAGER_H
