#pragma once

#include "Utils/DataUtils.h"
#include "Managers/DBmanager.h"

#include <QObject>
#include <cstdio>
#include <vector>
#include <QString>
#include <QVariant>

using namespace std;
using namespace ClassScheduler;

class SearchTeacherInfoController : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(QVariantList teacherInfoList MEMBER mTeacherInfoList NOTIFY teacherInfoListChanged)
    /*
     * teacherName 老师名字（昵称）
     * teacherCourseList 老师当前的课程
     */

    //teacherCourseList
    /*
     * date 日期
     * studentName 学生姓名
     * weekend 周几
     * school 学校
     * studentPhoneNubmer 学生电话
     * grade 年级
     * suject 学科
     * time 时间（24小时制）
     * type 面授/网课
     * courseTime 课时
     * studentFee 学生单价 金额/小时
     * teacherFee 老师单价 金额/小时
     */

    enum class OperateMode
    {
        None,
        WelcomePage,
        SearchTeacherInfo,
        ScheduleClass,
        CalcOneToOneMoney,
        CalcClassMoney
    };
    Q_ENUM(OperateMode)
    using OperateModes = std::vector<OperateMode>;

public:
    explicit SearchTeacherInfoController(DBManagerPtr dbManager, QObject* parent = nullptr);
    Q_INVOKABLE void initialize();

signals:
    void teacherInfoListChanged();

private:
    void refreshSearchTeacherInfo();
    void readTeacherInfosFromDB();
    void getTeacherInfosFromExcelFile(QString filePath);
    void readyForTeacherInfos();

private:
    QVariantList mTeacherInfoList;
    TeacherInfos mTeacherInfosFromDB;
    DBManagerPtr mDBManager;

};

