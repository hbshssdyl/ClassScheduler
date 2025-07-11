#include "UserManager.h"

UserManager::UserManager() {}

UserInfo UserManager::getUserInfoByLoginInfo(std::string& username, std::string& password, LoginInfos& loginInfos)
{
    UserInfo info;
    for(auto& loginInfo : loginInfos)
    {
        qDebug() << username << password;
        qDebug() << loginInfo.username << loginInfo.password;
        if(username == loginInfo.username && password == loginInfo.password)
        {
            info.name = getNameByUsername(username);
            info.mods = getModsByUserLevel(loginInfo.level);
            info.level = loginInfo.level;
            return info;
        }
    }
    return info;
}

OperateModes UserManager::getModsByUserLevel(UserLevel& level)
{
    switch (level) {
        case UserLevel::SuperAdmin:
            return { OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::ScheduleClass,
                    OperateMode::TeacherEvaluation};
        case UserLevel::Owner:
            return { OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::ScheduleClass};
        case UserLevel::Manager:
            return { OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::ScheduleClass};
        case UserLevel::SeniorUser:
            return { OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::ScheduleClass};
        case UserLevel::RegularUser:
            return { OperateMode::SearchClassInfo,
                    OperateMode::SearchTeacherInfo,
                    OperateMode::SearchStudentInfo,
                    OperateMode::ScheduleClass};
        default:
            return {};
            break;
    }
}

std::string UserManager::getNameByUsername(std::string username)
{
    if(username == "zhuhui")
        return "朱老师";
    if(username == "xiaoru")
        return "小如老师";
    if(username == "tingting")
        return "婷婷";
    if(username == "Dylan")
        return "超级管理员";
    if(username == "jiajia")
        return "佳佳";
    return "";
}
