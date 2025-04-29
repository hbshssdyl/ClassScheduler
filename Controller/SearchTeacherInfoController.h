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
    Q_PROPERTY(QVariantList teacherHeaderList MEMBER mTeacherHeaderList NOTIFY teacherInfoHeaderChanged)

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
    void teacherInfoHeaderChanged();

public slots:
    void onSearchTriggered(QString searchString);

private:
    void refreshSearchTeacherInfo();
    void readTeacherInfosFromDB();
    void initTeahcerHeader();
    void updateTeacherInfosList(TeacherInfos& infos);

private:
    QVariantList mTeacherInfoList;
    QVariantList mTeacherHeaderList;
    TeacherInfos mTeacherInfosFromDB;
    DBManagerPtr mDBManager;
    bool mIsDBDataExist = false;

};

