#pragma once

#include <QObject>
#include <iostream>
#include <cstdio>
#include <vector>
#include <QString>
#include <QVariant>

#include "Utils/DataUtils.h"

using namespace std;
using namespace PictureManager;

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
     * week 周几
     * school 学校
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
    void initialize();

public slots:
    void onOperateModeSelected(OperateMode mode);
    void onForderPathReceived(QString dirPath);
    void onDeleteRepeatedImagesAction();

signals:
    void operateModeChanged();
    void actionItemsListChanged();
    void repeatedImagesChanged();

private:
    std::string toOperateModeString(OperateMode mode);
    void refreshOperateMode(OperateMode mode);
    void handleSearchTeacherInfoMode();

private:
    OperateMode mOperateMode { OperateMode::None };
    QString mLoadedView { "" };
    QVariantList mActionItemsList;
    OperateModes mAllOperateMode;
    QVariantList mGroupRepeatedImages;
    vector<RepeatedImages> mRepeatedImagesGroup;

};

