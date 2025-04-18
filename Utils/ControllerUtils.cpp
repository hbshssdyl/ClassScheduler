#pragma once

#include "ControllerUtils.h"
#include "Controller/Controller.h"

namespace fs = std::filesystem;

namespace ClassScheduler
{

//For controller.cpp
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
/*
     * teacherName 老师名字
     * teacherCourseList 老师当前的课程
     */

//teacherCourseList
/*
     * suject 科目
     * date 日期
     * time 时间（24小时制）
     * week 周几
     * school 学校
     * grade 年级
     * courseTime 课时
     * studentFee 家长单价 金额/小时
     * teacherFee 老师单价 金额/小时
     */
void CUtils::updateTeacherInfoList(QVariantList& data)
{
    QVariantList teacherCourseList;
    QVariantMap teacherCourse1 = QVariantMap{ { "suject", "科目1" },
                                          { "date", "日期1" },
                                          { "time", "时间1（24小时制）" },
                                          { "week", "周几1" },
                                          { "school", "学校1" },
                                          { "grade", "年级1" },
                                          { "courseTime", "课时1" },
                                          { "studentFee", "家长单价1" },
                                          { "teacherFee", "老师单价1" }};
    QVariantMap teacherCourse2 = QVariantMap{ { "suject", "科目2" },
                                             { "date", "日期2" },
                                             { "time", "时间2（24小时制）" },
                                             { "week", "周几2" },
                                             { "school", "学校2" },
                                             { "grade", "年级2" },
                                             { "courseTime", "课时2" },
                                             { "studentFee", "家长单价2" },
                                             { "teacherFee", "老师单价2" }};
    QVariantMap teacherCourse3 = QVariantMap{ { "suject", "科目3" },
                                             { "date", "日期3" },
                                             { "time", "时间3（24小时制）" },
                                             { "week", "周几3" },
                                             { "school", "学校3" },
                                             { "grade", "年级3" },
                                             { "courseTime", "课时3" },
                                             { "studentFee", "家长单价3" },
                                             { "teacherFee", "老师单价3" }};
    teacherCourseList.append(teacherCourse1);
    teacherCourseList.append(teacherCourse2);
    teacherCourseList.append(teacherCourse3);
    data.append(QVariantMap{ { "teacherName", "老师1" },
                            { "teacherCourseList", teacherCourseList }});
    data.append(QVariantMap{ { "teacherName", "老师2" },
                            { "teacherCourseList", teacherCourseList }});
    data.append(QVariantMap{ { "teacherName", "老师3" },
                            { "teacherCourseList", teacherCourseList }});
}

} // namespace PictureManager
