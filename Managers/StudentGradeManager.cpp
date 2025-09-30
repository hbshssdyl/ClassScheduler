#include "StudentGradeManager.h"
#include "NetworkManager.h"
#include "CoreFramework.h"

StudentGradeManager::StudentGradeManager(CoreFrameworkPtr coreFramework)
    : mCoreFramework(coreFramework)
{
}

StudentGradeInfos StudentGradeManager::getStudentGradeInfos()
{
    return mStudentGradeInfos;
}

void StudentGradeManager::initialize()
{
    initStudentGradeInfos();
}

void StudentGradeManager::refreshDataFromServer()
{
    initStudentGradeInfos();
}

void StudentGradeManager::initStudentGradeInfos()
{
    mStudentGradeInfos = getStudentGradeInfosFromServer();
}

StudentGradeInfos StudentGradeManager::getStudentGradeInfosFromServer()
{
    LOG_INFO("getStudentGradeInfosFromServer ");
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            auto response = networkManager->getAllStudentGrades();
            LOG_INFO(response.rawResponse);
            if(response.status == ResultStatus::GetAllStudentGradeInfosSuccess)
            {
                return response.studentGradeInfos;
            }
            return {};
        }
    }
    LOG_INFO("no coreFramework or no networkManager");
    return {};
}

bool StudentGradeManager::addStudentInfo(const StudentGradeInfo& studentInfo)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            auto result = networkManager->createStudentInfo(studentInfo);
            LOG_INFO(result.statusStr);
            LOG_INFO(result.rawResponse);
            if(result.status == ResultStatus::CreateStudentInfoSuccess)
            {
                initStudentGradeInfos();
                return true;
            }
        }
    }
    return false;
}

bool StudentGradeManager::addStudentGrade(const Grade& grade, int studentId)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            auto result = networkManager->createStudentGrade(grade, studentId);
            LOG_INFO(result.statusStr);
            LOG_INFO(result.rawResponse);
            if(result.status == ResultStatus::CreateStudentGradeSuccess)
            {
                initStudentGradeInfos();
                return true;
            }
        }
    }
    return false;
}

bool StudentGradeManager::updateGrade(const Grade& grade)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            LOG_INFO("StudentGradeInfos size: " + std::to_string(mStudentGradeInfos.size()));
            // for(auto& feedbackInfo : mStudentGradeInfos)
            // {
            //     LOG_INFO("feedbackInfos.id: " +std::to_string(feedbackInfo.id) +", feedbackId: " +std::to_string(feedbackId) +", feedbackInfos.Status: " +to_string(feedbackInfo.feedbackStatus));
            //     if(feedbackInfo.id == feedbackId && feedbackInfo.feedbackStatus == FeedbackStatus::Submitted)
            //     {
            //         FeedbackUpdateStatusReq requestInfo;
            //         requestInfo.id = feedbackId;
            //         requestInfo.newStatus = FeedbackStatus::Approved;
            //         auto result = networkManager->updateFeedbackStatus(requestInfo);
            //         LOG_INFO(result.statusStr);
            //         LOG_INFO(result.rawResponse);
            //         if(result.status == ResultStatus::UpdateFeedbackStatusSuccess)
            //         {
            //             feedbackInfo.feedbackStatus = FeedbackStatus::Approved;
            //         }
            //         return true;
            //     }
            // }
        }
    }
    return false;
}

bool StudentGradeManager::deleteGrade(int gradeId)
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            LOG_INFO("StudentGradeInfos size: " + std::to_string(mStudentGradeInfos.size()));
            auto result = networkManager->deleteGrade(gradeId);
            LOG_INFO(result.statusStr);
            LOG_INFO(result.rawResponse);
            if(result.status == ResultStatus::DeleteStudentGradeSuccess)
            {
                initStudentGradeInfos();
                return true;
            }
        }
    }
    return false;
}
