#pragma once

#include <String>
#include <vector>
#include <QString>
#include <map>
#include <QVariant>
#include <algorithm>

using namespace std;

namespace ClassScheduler
{
    static const QString CLASS_INFOS_TABLE_NAME = "classInfos";
    static const QString TEACHER_INFOS_TABLE_NAME = "teacherInfos";

    static QString nullString = "无该信息";
    static vector validExcelClassHeader{"日期", "星期", "姓名", "学校", "电话", "年级", "学科", "时间", "老师", "网课or面授", "课时", "金额/小时", "课酬总计", "老师姓名", "老师工资", "已收金额", "付费方式", "收费日期"};
    static vector allTableNameForDB{"classInfos", "teacherInfos"};

    // QVariantList schedulerOptions {
    //     QVariantMap{
    //         {"isMust", true},
    //         {"placeholder", "选择科目"},
    //         {"options", QVariantList{"语文", "数学", "英语", "物理", "化学", "生物", "道法"}}
    //     },
    //     QVariantMap{
    //         {"isMust", true},
    //         {"placeholder", "选择年级"},
    //         {"options", QVariantList{"小学", "初一", "初二", "初三", "高一", "高二", "高三"}}
    //     },
    //     QVariantMap{
    //         {"isMust", true},
    //         {"placeholder", "选择星期"},
    //         {"options", QVariantList{"星期一", "星期二", "星期三", "星期四", "星期五", "星期六", "星期日"}}
    //     }
    // };

    struct ClassInfo
    {
        QString date;
        QString weekend;
        QString studentName;
        QString school;
        QString studentPhoneNubmer;
        QString grade;

        QString suject;
        QString time;
        QString teacherNickName;
        QString learningType;
        QString courseTime;
        QString studentFee;

        QString studentTotalFee;
        QString teacherName;
        QString teacherFee;
        QString gotMoney;
        QString payType;
        QString payDate;

        QVariantList toInfosList(QString id)
        {
            QVariantList list;
            if(!id.isEmpty())
            {
                list.append(id);
            }
            list.append(date);list.append(weekend);list.append(studentName);list.append(school);list.append(studentPhoneNubmer);list.append(grade);
            list.append(suject);list.append(time);list.append(teacherNickName);list.append(learningType);list.append(courseTime);list.append(studentFee);
            list.append(studentTotalFee);list.append(teacherName);list.append(teacherFee);list.append(gotMoney);list.append(payType);list.append(payDate);
            return list;
        }

        bool isValidInfo()
        {
            int cnt = 0;
            if(studentName == nullString) return false;
            if(nullString != teacherName) cnt++;
            if(nullString != teacherNickName) cnt++;
            if(nullString != date) cnt++;
            if(nullString != weekend) cnt++;
            if(nullString != studentName) cnt++;
            if(nullString != school) cnt++;
            if(nullString != studentPhoneNubmer) cnt++;
            if(nullString != grade) cnt++;
            if(nullString != suject) cnt++;
            if(nullString != time) cnt++;
            if(nullString != learningType) cnt++;
            if(nullString != courseTime) cnt++;
            if(nullString != studentFee) cnt++;
            if(nullString != studentTotalFee) cnt++;
            if(nullString != teacherFee) cnt++;
            if(nullString != gotMoney) cnt++;
            if(nullString != payType) cnt++;
            if(nullString != payDate) cnt++;
            if(cnt < 5) return false;
            return true;
        }

        bool isContains(QString str)
        {
            int cnt = 0;
            if(date.contains(str, Qt::CaseSensitive)) return true;
            if(weekend.contains(str, Qt::CaseSensitive)) return true;
            if(studentName.contains(str, Qt::CaseSensitive)) return true;
            if(school.contains(str, Qt::CaseSensitive)) return true;
            if(studentPhoneNubmer.contains(str, Qt::CaseSensitive)) return true;
            if(grade.contains(str, Qt::CaseSensitive)) return true;

            if(suject.contains(str, Qt::CaseSensitive)) return true;
            if(time.contains(str, Qt::CaseSensitive)) return true;
            if(teacherNickName.contains(str, Qt::CaseSensitive)) return true;
            if(learningType.contains(str, Qt::CaseSensitive)) return true;
            if(courseTime.contains(str, Qt::CaseSensitive)) return true;
            if(studentFee.contains(str, Qt::CaseSensitive)) return true;

            if(studentTotalFee.contains(str, Qt::CaseSensitive)) return true;
            if(teacherName.contains(str, Qt::CaseSensitive)) return true;
            if(teacherFee.contains(str, Qt::CaseSensitive)) return true;
            if(gotMoney.contains(str, Qt::CaseSensitive)) return true;
            if(payType.contains(str, Qt::CaseSensitive)) return true;
            if(payDate.contains(str, Qt::CaseSensitive)) return true;
            return false;
        }
    };

    struct TeacherInfo
    {
        QString teacherName;
        vector<QString> teacherNickNames;
        vector<QString> teacherFees;
        vector<QString> teacherSujects;
        vector<QString> teacherGrades;

        TeacherInfo(const QString teacherName)
            : teacherName(teacherName)
        {
        }

        void sortInfos()
        {
            sort(teacherNickNames.begin(), teacherNickNames.end());
            sort(teacherFees.begin(), teacherFees.end());
            sort(teacherSujects.begin(), teacherSujects.end());
            sort(teacherGrades.begin(), teacherGrades.end());
        }

        QString getString(vector<QString> stringList)
        {
            QString ret = "";
            bool flag = false;
            for(auto& str : stringList)
            {
                if(flag)
                {
                    ret += ", ";
                }
                ret += str;
                flag = true;
            }
            return ret;
        }

        void saveValue(QString value, vector<QString>& values)
        {
            bool save = true;
            for(auto& val : values)
            {
                if(value == val)
                {
                    save = false;
                    break;
                }
            }
            if(save)
            {
                values.emplace_back(value);
            }
        }

        QString getTeacherNickNames()
        {
            return getString(teacherNickNames);
        }

        QString getTeacherFees()
        {
            return getString(teacherFees);
        }

        QString getTeacherSujects()
        {
            return getString(teacherSujects);
        }

        QString getTeacherGrades()
        {
            return getString(teacherGrades);
        }
    };

    using ClassInfos = vector<ClassInfo>;
    using TeacherInfos = vector<TeacherInfo>;
    using TableDataCount = std::map<QString, int>; //QString tableName, int dataCount
} // namespace ClassScheduler
