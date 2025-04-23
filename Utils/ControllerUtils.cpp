#pragma once

#include "DataUtils.h"
#include "ControllerUtils.h"
#include "Controller/Controller.h"
#include "xlsxdocument.h"
// #include "xlsxchartsheet.h"
// #include "xlsxcellrange.h"
// #include "xlsxchart.h"
// #include "xlsxrichstring.h"
#include "xlsxworkbook.h"

using namespace QXlsx;
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

bool hasValidHeaders(Document& doc)
{
    int row = 1, col = 1;
    map<std::string, bool> headers;
    while(col)
    {
        QVariant var = doc.read(row, col);
        auto str = var.toString().toStdString();
        if(str.empty())
        {
            break;
        }
        headers[str] = true;
        col++;
    }
    for(auto header : validExcelHeader)
    {
        if(!headers[header])
        {
            return false;
        }
    }
    return true;
}

bool saveData(teacherInfo& info, std::string& headerStr, std::string& str)
{
    if(headerStr.empty() || str.empty())
    {
        return false;
    }
    cout << str << endl;
    if(headerStr == "日期") info.date = str;
    else if(headerStr == "姓名") info.studentName = str;
    else if(headerStr == "学校") info.school = str;
    else if(headerStr == "电话") info.studentPhoneNubmer = str;
    else if(headerStr == "年级") info.grade = str;
    else if(headerStr == "学科") info.suject = str;
    else if(headerStr == "时间") info.time = str;
    else if(headerStr == "老师") info.teacherNickName = str;
    else if(headerStr == "网课or面授") info.type = str;
    else if(headerStr == "金额/小时") info.studentFee = str;
    else if(headerStr == "老师姓名") info.teacherName = str;
    else if(headerStr == "老师工资") info.teacherFee = str;
    return true;
}

bool isValidTeacherInfo(teacherInfo& info)
{
    if(info.studentName.empty()) return false;
    if(info.teacherNickName.empty()) return false;
    if(info.time.empty()) return false;
    return true;
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

void CUtils::getTeacherInfosFromExcelFile(vector<teacherInfo>& teacherInfos, QString filePath)
{
    Document doc(filePath);
    if (!doc.load())
        return;

    if(!hasValidHeaders(doc))
    {
        return;
    }

    int row = 2; int col = 1;
    while(row)
    {
        col = 1;
        teacherInfo info;
        while(col)
        {
            QVariant header = doc.read(1, col);
            auto headerStr = header.toString().toStdString();
            QVariant var = doc.read(row, col);
            auto str = var.toString().toStdString();

            if(!saveData(info, headerStr, str))
            {
                break;
            }

            col++;
        }
        if(!isValidTeacherInfo(info))
        {
            break;
        }
        teacherInfos.emplace_back(info);
        row++;
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
void CUtils::updateTeacherInfoList(QVariantList& data, vector<teacherInfo> teacherInfos)
{
    QVariantList teacherCourseList;
    QVariantMap teacherCourse1 = QVariantMap{ { "suject", "test" },
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
