#pragma once

#include "Utils/DataUtils.h"
#include "Managers/DBManager.h"

#include <QObject>
#include <cstdio>
#include <vector>
#include <QString>
#include <QVariant>

using namespace std;
using namespace ClassScheduler;

class SearchClassInfoController : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(QVariantList classInfoList MEMBER mClassInfoList NOTIFY classInfoListChanged)
    Q_PROPERTY(QVariantList classHeaderList MEMBER mClassHeaderList NOTIFY classInfoHeaderChanged)

    enum class OperateMode
    {
        None,
        WelcomePage,
        SearchClassInfo,
        ScheduleClass,
        CalcOneToOneMoney,
        CalcClassMoney
    };
    Q_ENUM(OperateMode)
    using OperateModes = std::vector<OperateMode>;

public:
    explicit SearchClassInfoController(DBManagerPtr dbManager, QObject* parent = nullptr);
    Q_INVOKABLE void initialize();

signals:
    void classInfoListChanged();
    void classInfoHeaderChanged();

public slots:
    void onSearchTriggered(QString searchString);

private:
    void refreshSearchClassInfo();
    void readClassInfosFromDB();
    void initClassHeader();
    void updateClassInfosList(ClassInfos& infos);

private:
    QVariantList mClassInfoList;
    QVariantList mClassHeaderList;
    ClassInfos mClassInfosFromDB;
    DBManagerPtr mDBManager;
    bool mIsDBDataExist = false;

};

