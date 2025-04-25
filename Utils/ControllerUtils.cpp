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
    if(str.isEmpty() || str == "00:00:00.000")
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
