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



QVariantMap getTeacherListInfo(int id, TeacherInfo teacherInfo)
{
    return QVariantMap{{ "id", QString::number(id) },
                       { "date", teacherInfo.date },
                       { "weekend", teacherInfo.weekend },
                       { "studentName", teacherInfo.studentName },
                       { "school", teacherInfo.school },
                       { "studentPhoneNubmer", teacherInfo.studentPhoneNubmer },
                       { "grade", teacherInfo.grade },

                       { "suject", teacherInfo.suject },
                       { "time", teacherInfo.time },
                       { "teacherNickName", teacherInfo.teacherNickName },
                       { "learningType", teacherInfo.learningType },
                       { "courseTime", teacherInfo.courseTime },
                       { "studentFee", teacherInfo.studentFee },

                       { "studentTotalFee", teacherInfo.studentTotalFee },
                       { "teacherName", teacherInfo.teacherName },
                       { "teacherFee", teacherInfo.teacherFee },
                       { "gotMoney", teacherInfo.gotMoney },
                       { "payType", teacherInfo.payType },
                       { "payDate", teacherInfo.payDate }};
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

//For SearchTeacherInfoController.cpp
void CUtils::updateTeacherInfoList(QVariantList& data, TeacherInfos& teacherInfos)
{
    int id = 1;
    for(auto teacherInfo : teacherInfos)
    {
        auto info = getTeacherListInfo(id++, teacherInfo);
        data.append(info);
    }
}

void CUtils::updateTeacherHeaderList(QVariantList& data)
{
    data.append("序号");
    for(auto header : validExcelHeader)
    {
        data.append(header);
    }
}

} // namespace PictureManager
