#pragma once

#include <QObject>
#include <iostream>
#include <cstdio>
#include <vector>
#include <QString>
#include <QVariant>

using namespace std;

class SearchTeacherInfoController : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(QVariantList teacherInfoList MEMBER mTeacherInfoList NOTIFY teacherInfoListChanged)
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
    explicit SearchTeacherInfoController(QObject* parent = nullptr);
    Q_INVOKABLE void initialize();

signals:
    void teacherInfoListChanged();

private:
    void refreshSearchTeacherInfo();

private:
    QVariantList mTeacherInfoList;

};

