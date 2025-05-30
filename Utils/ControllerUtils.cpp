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
    case Controller::OperateMode::SearchStudentInfo:
        return "查询学生信息";
    case Controller::OperateMode::ScheduleClass:
        return "排课";
    case Controller::OperateMode::TeacherEvaluation:
        return "老师评价系统";
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

bool isCurrentMonth(const QString& dateStr) {
    //return true;
    QDate inputDate = QDate::fromString(dateStr, "yyyy-MM-dd");
    if (!inputDate.isValid()) return false;
    QDate currentDate = QDate::currentDate();
    return (inputDate.year() == currentDate.year() && inputDate.month() == currentDate.month() - 2);
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
void CUtils::updateClassInfoList(QVariantMap& data, ClassInfos& classInfos)
{
    QVariantList classInfoHeader;
    QVariantList classInfoList;
    int id = 1;

    classInfoHeader.append("序号");
    for(auto header : validExcelClassHeader)
    {
        classInfoHeader.append(header);
    }

    for(auto& classInfo : classInfos)
    {
        classInfoList.append(classInfo.toInfosList(QString::number(id++)));
    }

    data.insert("classInfoHeader", classInfoHeader);
    data.insert("classInfoList", classInfoList);
}

void CUtils::doSearchClassInfos(ClassInfos& allInfos, ClassInfos& searchInfos, QString searchString)
{
    for(auto& info : allInfos)
    {
        if(info.isAllContains(searchString))
        {
            searchInfos.emplace_back(info);
        }
    }
}

//For SearchTeacherInfoController.cpp
void CUtils::updateTeacherInfoList(QVariantMap& data, TeacherInfos& teacherBasicInfos)
{
    if(teacherBasicInfos.size() == 0)
    {
        return;
    }
    QVariantList teacherInfoHeader;
    QVariantList teacherInfoList;

    //update teacherInfoHeader
    teacherInfoHeader.append("序号");
    for(auto header : validTeacherHeader)
    {
        teacherInfoHeader.append(header);
    }

    //update teacherInfoList
    int id = 1;
    for(auto& teacherBasicInfo : teacherBasicInfos)
    {
        QVariantList sujectStudentInfoList;
        for(auto& info : teacherBasicInfo.sujectStudentCounts)
        {
            sujectStudentInfoList.append(info.toMapStyle());
        }
        teacherInfoList.append(QVariantMap{ { "basicInfo", teacherBasicInfo.toInfosList(QString::number(id++)) },
                                            { "studentInfo", sujectStudentInfoList } });
    }

    data.insert("teacherInfoHeader", teacherInfoHeader);
    data.insert("teacherInfoList", teacherInfoList);

    //update studentBasicInfo
    data.insert("maxStudentCount", teacherBasicInfos[0].teacherStudentCountBasicInfo.maxKeyCount);
    data.insert("minYearMonth", teacherBasicInfos[0].teacherStudentCountBasicInfo.minYearMonth);
    data.insert("maxYearMonth", teacherBasicInfos[0].teacherStudentCountBasicInfo.maxYearMonth);
}

void CUtils::doSearchTeacherInfos(TeacherInfos& allInfos, TeacherInfos& searchInfos, QString searchString)
{
    for(auto& info : allInfos)
    {
        if(info.isAllContains(searchString))
        {
            searchInfos.emplace_back(info);
        }
    }
}

//For SearchStudentInfoTeacherInfoController.cpp
void CUtils::updateStudentInfoList(QVariantMap& data, StudentInfos& teacherBasicInfos/*, TeacherStudentInfos& teacherStudentInfos, TeacherStudentBasicInfo& studentBasicInfo*/)
{
    // QVariantList teacherInfoHeader;
    // QVariantList teacherInfoList;

    // //update teacherInfoHeader
    // teacherInfoHeader.append("序号");
    // for(auto header : validTeacherHeader)
    // {
    //     teacherInfoHeader.append(header);
    // }

    // //update teacherInfoList
    // int id = 1;
    // for(auto& teacherBasicInfo : teacherBasicInfos)
    // {
    //     QVariantList sujectStudentInfoList;
    //     for(auto& teacherStudentInfo : teacherStudentInfos)
    //     {
    //         if(teacherBasicInfo.teacherName == teacherStudentInfo.teacherName)
    //         {
    //             for(auto& info : teacherStudentInfo.sujectStudentInfos)
    //             {
    //                 sujectStudentInfoList.append(info.toMapStyle());
    //             }
    //         }
    //     }
    //     teacherInfoList.append(QVariantMap{ { "basicInfo", teacherBasicInfo.toInfosList(QString::number(id++)) },
    //                                        { "studentInfo", sujectStudentInfoList } });
    // }

    // data.insert("teacherInfoHeader", teacherInfoHeader);
    // data.insert("teacherInfoList", teacherInfoList);

    // //update studentBasicInfo
    // data.insert("maxStudentCount", studentBasicInfo.maxStudentCount);
    // data.insert("minYearMonth", studentBasicInfo.minYearMonth);
    // data.insert("maxYearMonth", studentBasicInfo.maxYearMonth);
}

void CUtils::doSearchStudentInfos(StudentInfos& allInfos, StudentInfos& searchInfos, QString searchString)
{
    for(auto& info : allInfos)
    {
        if(info.isAllContains(searchString))
        {
            searchInfos.emplace_back(info);
        }
    }
}

//For ScheduleClassController.cpp
bool CUtils::isTimeOverlap(const QString& timeRange1, const QString& timeRange2) {
    QStringList range1 = timeRange1.split('-');
    QStringList range2 = timeRange2.split('-');

    if (range1.size() != 2 || range2.size() != 2) {
        return false;
    }

    QTime start1 = QTime::fromString(range1[0], "hh:mm");
    QTime end1 = QTime::fromString(range1[1], "hh:mm");
    QTime start2 = QTime::fromString(range2[0], "hh:mm");
    QTime end2 = QTime::fromString(range2[1], "hh:mm");

    if (!start1.isValid() || !end1.isValid() || !start2.isValid() || !end2.isValid()) {
        cout << "Invalid time format" << endl;
        return false; // Invalid time format
    }

    if(start1 <= start2 && end1 >= end2)
    {
        return true;
    }

    if(start2 <= start1 && end2 >= end1)
    {
        return true;
    }

    if(start2 >= start1 && start2 < end1 && end2 >= end1)
    {
        return true;
    }

    if(start1 >= start2 && start1 < end2 && end1 >= end2)
    {
        return true;
    }

    return false;
}

void CUtils::updateScheduleClassResultsList(QList<QList<QVariantMap>>& data, ScheduleClassInputInfo& inputInfos, ClassInfos& classInfosList)
{
    map<QString, bool> isValidTeacher;

    for(auto& classInfo : classInfosList)
    {
        if(!isCurrentMonth(classInfo.date))
        {
            continue;
        }

        if(isValidTeacher.count(classInfo.teacherName) == 1)
        {
            if(!isValidTeacher[classInfo.teacherName])
            {
                continue;
            }
        }

        if(classInfo.suject == inputInfos.suject)
        {
            isValidTeacher[classInfo.teacherName] = true;
            if(classInfo.weekend == inputInfos.week)
            {
                if(isTimeOverlap(classInfo.time, inputInfos.timeRange))
                {
                    cout << "TimeOverlap: classInfo.time: " << classInfo.time.toStdString() << ", inputInfos.timeRange: " << inputInfos.timeRange.toStdString() << endl;
                    isValidTeacher[classInfo.teacherName] = false;
                }
            }
        }
    }

    ScheduleClassResultInfos resultInfoList;
    for(auto& classInfo : classInfosList)
    {
        if(!isCurrentMonth(classInfo.date))
        {
            continue;
        }

        if(isValidTeacher[classInfo.teacherName] && inputInfos.suject == classInfo.suject)
        {
            bool isNewTeacher = true;
            for(auto& result : resultInfoList)
            {
                if(result.teacherName == classInfo.teacherName)
                {
                    isNewTeacher = false;
                    result.saveValue(classInfo.teacherNickName, result.teacherNickNames);
                    result.saveValue(classInfo.grade + "_" + classInfo.teacherFee, result.teacherGradeFees);

                    if(classInfo.weekend == inputInfos.week)
                    {
                        //result.saveValue(classInfo.grade, result.teacherWorkGrade);
                        result.saveValue(classInfo.time, result.teacherWorkTime, false);
                    }
                    break;
                }
            }
            if(isNewTeacher)
            {
                ScheduleClassResultInfo info;
                info.teacherName = classInfo.teacherName;
                info.week = inputInfos.week;
                info.teacherNickNames.emplace_back(classInfo.teacherNickName);
                info.teacherGradeFees.emplace_back(classInfo.grade + "_" + classInfo.teacherFee);

                if(classInfo.weekend == inputInfos.week)
                {
                    info.teacherWorkTime.emplace_back(classInfo.time);
                    //info.teacherWorkGrade.emplace_back(classInfo.grade);
                }
                resultInfoList.emplace_back(info);
            }
        }
    }

    for(auto& result : resultInfoList)
    {
        result.generateValidWorkTime();
        result.generateTeacherFreeTime();
        result.generateValidFreeTime(inputInfos.timeLength);
        result.sortInfos();
        result.generateInfoString();
    }

    for(auto& result : resultInfoList)
    {
        if(!result.isValid(inputInfos.timeRange))
        {
            continue;
        }

        QList<QVariantMap> tmp;
        tmp.append(QVariantMap{ { "label", "姓　　名： " },
                                 { "value", result.teacherName } });

        tmp.append(QVariantMap{ { "label", "昵　　称： " },
                                 { "value", result.strTeacherNickNames } });

        tmp.append(QVariantMap{ { "label", "年级费用： " },
                                 { "value", result.strTeacherGradeFees } });

        // tmp.append(QVariantMap{ { "label", "教过年级： " },
        //                          { "value", result.strTeacherWorkTime } });

        tmp.append(QVariantMap{ { "label", "有课时间： " },
                                 { "value", result.strTeacherValidWorkTime } });

        tmp.append(QVariantMap{ { "label", "无课时间： " },
                                 { "value", result.strTeacherValidFreeTime } });
        data.append(tmp);
    }
}

} // namespace PictureManager
