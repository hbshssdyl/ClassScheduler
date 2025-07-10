#include "UserManager.h"

UserManager::UserManager() {}

UserInfo UserManager::getUserInfoByLoginInfo(std::string& username, std::string& password)
{

}

OperateModes getModsByUserLevel(UserLevel level)
{
    switch (level) {
        case UserLevel::SuperAdmin:
            return { OperateMode::TeacherEvaluation,
                    OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::ScheduleClass};
        default:
            break;
    }
}
