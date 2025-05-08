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

class SearchClassInfoController : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(QVariantMap classInfoMap MEMBER mClassInfoMap NOTIFY classInfoMapChanged)

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
    explicit SearchClassInfoController(DataManagerPtr DataManager, QObject* parent = nullptr);
    void refreshSearchClassInfo();

signals:
    void classInfoMapChanged();

public slots:
    void onSearchTriggered(QString searchString);

private:
    void initialize();
    void updateClassInfosList(ClassInfos& infos);

private:
    QVariantMap mClassInfoMap;
    ClassInfos mClassInfosFromDB;
    DataManagerPtr mDataManager;

};

