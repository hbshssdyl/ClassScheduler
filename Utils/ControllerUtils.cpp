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
using CellPtr = std::shared_ptr<Cell>;

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
    map<QString, bool> headers;
    while(col)
    {
        CellPtr cell = doc.cellAt(row, col);
        if(!cell)
        {
            break;
        }
        QVariant var = cell->value();
        auto str = var.toString();
        //cout << str << endl;
        if(str.isEmpty())
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
            cout << "无该信息: " << header << endl;
            return false;
        }
    }
    return true;
}

void saveData(TeacherInfo& info, QString& headerStr, QString& str)
{
    if(str.isEmpty())
    {
        str = nullString;
    }
    if(headerStr == "日期") info.date = str;
    else if(headerStr == "星期") info.weekend = str;
    else if(headerStr == "姓名") info.studentName = str;
    else if(headerStr == "学校") info.school = str;
    else if(headerStr == "电话") info.studentPhoneNubmer = str;
    else if(headerStr == "年级") info.grade = str;
    else if(headerStr == "学科") info.suject = str;
    else if(headerStr == "时间") info.time = str;
    else if(headerStr == "老师") info.teacherNickName = str;
    else if(headerStr == "网课or面授") info.learningType = str;
    else if(headerStr == "课时") info.courseTime = str;
    else if(headerStr == "金额/小时") info.studentFee = str;
    else if(headerStr == "课酬总计") info.studentTotalFee = str;
    else if(headerStr == "老师姓名") info.teacherName = str;
    else if(headerStr == "老师工资") info.teacherFee = str;
    else if(headerStr == "已收金额") info.gotMoney = str;
    else if(headerStr == "付费方式") info.payType = str;
    else if(headerStr == "收费日期") info.payDate = str;
}

bool isUsefulHeader(QString header)
{
    for(auto str : validExcelHeader)
    {
        if(header == str) return true;
    }
    return false;
}

QVariant readCellValue(QString headerStr, CellPtr cell)
{
    if(headerStr == "星期")
    {
        return cell->value();
    }
    return cell->readValue();
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

TeacherInfos CUtils::getTeacherInfosFromExcelFile(QString filePath)
{
    TeacherInfos infos;
    Document doc(filePath);
    if (!doc.load())
        return infos;

    if(!hasValidHeaders(doc))
    {
        return infos;
    }

    int row = 2; int col = 1;
    while(row)
    {
        col = 1;
        TeacherInfo info;
        while(col)
        {
            CellPtr headerCell = doc.cellAt(1, col);
            if(!headerCell)
            {
                break;
            }
            QVariant header = headerCell->readValue();

            auto headerStr = header.toString();
            //cout << row << " " << col << " " << headerStr << " ";
            if(headerStr.isEmpty())
            {
                break;
            }
            if(!isUsefulHeader(headerStr))
            {
                col++;
                continue;
            }

            CellPtr cell = doc.cellAt(row, col);
            if(!cell)
            {
                col++;
                continue;
            }
            QVariant var = readCellValue(headerStr, cell);
            auto str = var.toString();
            //cout << str << endl;
            saveData(info, headerStr, str);
            col++;
        }
        if(!info.isValidInfo())
        {
            break;
        }
        infos.emplace_back(info);
        row++;
    }
    return infos;
}

//For SearchTeacherInfoController.cpp
void CUtils::updateTeacherInfoList(QVariantList& data, TeacherInfos& teacherInfos)
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
