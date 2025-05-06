#pragma once

#include "DataUtils.h"
#include "ControllerUtils.h"
#include "Controller/Controller.h"

namespace fs = std::filesystem;
namespace ClassScheduler
{

QString toOperateModeString(Controller::OperateMode mode)
{
    switch (mode)
    {
    case Controller::OperateMode::None:
        return "None";
    case Controller::OperateMode::WelcomePage:
        return "欢迎使用";
    case Controller::OperateMode::SearchClassInfo:
        return "查询课程信息";
    case Controller::OperateMode::SearchTeacherInfo:
        return "查询教师信息";
    case Controller::OperateMode::ScheduleClass:
        return "排课";
    case Controller::OperateMode::CalcOneToOneMoney:
        return "计算一对一费用";
    case Controller::OperateMode::CalcClassMoney:
        return "计算班课费用";
    default:
        return "default";
    }

}



QVariantMap getClassListInfo(int id, ClassInfo classInfo)
{
    return QVariantMap{{ "id", QString::number(id) },
                       { "date", classInfo.date },
                       { "weekend", classInfo.weekend },
                       { "studentName", classInfo.studentName },
                       { "school", classInfo.school },
                       { "studentPhoneNubmer", classInfo.studentPhoneNubmer },
                       { "grade", classInfo.grade },

                       { "suject", classInfo.suject },
                       { "time", classInfo.time },
                       { "teacherNickName", classInfo.teacherNickName },
                       { "learningType", classInfo.learningType },
                       { "courseTime", classInfo.courseTime },
                       { "studentFee", classInfo.studentFee },

                       { "studentTotalFee", classInfo.studentTotalFee },
                       { "teacherName", classInfo.teacherName },
                       { "teacherFee", classInfo.teacherFee },
                       { "gotMoney", classInfo.gotMoney },
                       { "payType", classInfo.payType },
                       { "payDate", classInfo.payDate }};
}

//For controller.cpp
void CUtils::updateActionItemsList(QVariantList& data, const Controller::OperateMode& selectedMode, const Controller::OperateModes& actionItems)
{
    for(auto activeItem : actionItems)
    {
        data.append(QVariantMap{ { "actionName", toOperateModeString(activeItem) },
                                { "OperateMode", static_cast<int>(activeItem) },
                                { "isSelected", activeItem == selectedMode }});
    }
}

//For SearchClassInfoController.cpp
void CUtils::updateClassInfoList(QVariantList& data, ClassInfos& classInfos)
{
    int id = 1;
    for(auto classInfo : classInfos)
    {
        auto info = getClassListInfo(id++, classInfo);
        data.append(info);
    }
}

void CUtils::doSearchClassInfos(ClassInfos& allInfos, ClassInfos& searchInfos, QString searchString)
{
    for(auto& info : allInfos)
    {
        if(info.isContains(searchString))
        {
            searchInfos.emplace_back(info);
        }
    }
}

void CUtils::updateClassHeaderList(QVariantList& data)
{
    data.append("序号");
    for(auto header : validExcelClassHeader)
    {
        data.append(header);
    }
}

//For SearchTeacherInfoController.cpp
void CUtils::updateTeacherInfoList(QVariantList& data, TeacherInfos& teacherInfos)
{
    // int id = 1;
    // for(auto classInfo : classInfos)
    // {
    //     auto info = getTeacherListInfo(id++, classInfo);
    //     data.append(info);
    // }
}

void CUtils::doSearchTeacherInfos(TeacherInfos& allInfos, TeacherInfos& searchInfos, QString searchString)
{
    // for(auto& info : allInfos)
    // {
    //     if(info.isContains(searchString))
    //     {
    //         searchInfos.emplace_back(info);
    //     }
    // }
}

void CUtils::updateTeacherHeaderList(QVariantList& data)
{
    // data.append("序号");
    // for(auto header : validExcelClassHeader)
    // {
    //     data.append(header);
    // }
}

} // namespace PictureManager
