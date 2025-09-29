#include "UserManager.h"
#include "CoreFramework.h"

UserManager::UserManager(CoreFrameworkPtr coreFramework)
    : mCoreFramework(coreFramework)
{
}

UserInfo UserManager::getUserInfoByLoginInfo(std::string& username, UserRole& role)
{
    UserInfo info;
    info.name = username;
    info.mods = getModsByUserRole(role);
    info.role = role;
    return info;
}

OperateModes UserManager::getModsByUserRole(UserRole& role)
{
    switch (role) {
        case UserRole::SuperAdmin:
            return { OperateMode::TaskAssistantView,
                    OperateMode::TaskManagerView,
                    OperateMode::FileView,
                    OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::StudentGradeView,
                    OperateMode::ScheduleClass,
                    OperateMode::TeacherEvaluation,
                    OperateMode::FeedbackApp,
                    OperateMode::FeedbackCompany,
                    OperateMode::AccountView };
        case UserRole::Boss:
            return { OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::StudentGradeView,
                    OperateMode::ScheduleClass,
                    OperateMode::FeedbackApp,
                    OperateMode::FeedbackCompany};
        case UserRole::Manager:
            return { OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::StudentGradeView,
                    OperateMode::ScheduleClass,
                    OperateMode::FeedbackApp,
                    OperateMode::FeedbackCompany};
        case UserRole::SeniorStaff:
            return { OperateMode::ScheduleClass,
                    OperateMode::FeedbackApp,
                    OperateMode::FeedbackCompany};
        case UserRole::Staff:
            return { OperateMode::FeedbackApp,
                    OperateMode::FeedbackCompany};
        case UserRole::OneToOneAssistant:
            return { OperateMode::TaskAssistantView,
                    OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::StudentGradeView,
                    OperateMode::ScheduleClass,
                    OperateMode::FeedbackApp,
                    OperateMode::FeedbackCompany};
        case UserRole::OneToOneManager:
            return { OperateMode::TaskManagerView,
                    OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::StudentGradeView,
                    OperateMode::ScheduleClass,
                    OperateMode::FeedbackApp,
                    OperateMode::FeedbackCompany};
        case UserRole::OneToMultiAssistant:
            return { OperateMode::TaskAssistantView,
                    OperateMode::StudentGradeView,
                    OperateMode::ScheduleClass,
                    OperateMode::FeedbackApp,
                    OperateMode::FeedbackCompany};
        case UserRole::OneToMultiManager:
            return { OperateMode::TaskManagerView,
                    OperateMode::StudentGradeView,
                    OperateMode::ScheduleClass,
                    OperateMode::FeedbackApp,
                    OperateMode::FeedbackCompany};
        default:
            return {};
            break;
    }
}
