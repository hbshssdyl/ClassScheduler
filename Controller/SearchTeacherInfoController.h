#pragma once

#include "Utils/DataUtils.h"
#include "Managers/DataManager.h"

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
    Q_PROPERTY(QVariantMap teacherInfoMap MEMBER mTeacherInfoMap NOTIFY teacherInfoMapChanged)

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
    explicit SearchTeacherInfoController(DataManagerPtr DataManager, QObject* parent = nullptr);
    void refreshSearchTeacherInfo();

signals:
    void teacherInfoMapChanged();

public slots:
    void onSearchTriggered(QString searchString);

private:
    void initialize();
    void updateTeacherInfosList(TeacherInfos& infos);

private:
    QVariantMap mTeacherInfoMap;
    TeacherInfos mTeacherInfosFromDB;
    DataManagerPtr mDataManager;

};

