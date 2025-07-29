#include "UserManager.h"

UserManager::UserManager() {}

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
            return { OperateMode::FileView,
                    OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::ScheduleClass,
                    OperateMode::TeacherEvaluation};
        case UserRole::Boss:
            return { OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::ScheduleClass};
        case UserRole::Manager:
            return { OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::ScheduleClass};
        case UserRole::SeniorStaff:
            return { OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::ScheduleClass};
        case UserRole::Staff:
            return { OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::ScheduleClass};
        default:
            return {};
            break;
    }
}
