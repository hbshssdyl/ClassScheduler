#pragma once

#include <QObject>
#include <cstdio>
#include <vector>
#include <QString>
#include <QVariant>
#include <QPointer>

#include "SearchTeacherInfoController.h"

using namespace std;

class Controller : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(OperateMode operateMode MEMBER mOperateMode NOTIFY operateModeChanged)
    Q_PROPERTY(QString loadedView MEMBER mLoadedView NOTIFY operateModeChanged)
    Q_PROPERTY(QVariantList actionItemsList MEMBER mActionItemsList NOTIFY actionItemsListChanged)

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
    explicit Controller(QObject* parent = nullptr);
    void initialize();

    Q_INVOKABLE virtual SearchTeacherInfoController* getSearchTeacherInfoController();

public slots:
    void onOperateModeSelected(OperateMode mode);

signals:
    void operateModeChanged();
    void actionItemsListChanged();

private:
    std::string toOperateModeString(OperateMode mode);
    void refreshOperateMode(OperateMode mode);
    void getTeacherInfosByExcelFile(QString filePath);

private:
    OperateMode mOperateMode { OperateMode::None };
    QString mLoadedView { "" };
    QVariantList mActionItemsList;
    OperateModes mAllOperateMode;
    QPointer<SearchTeacherInfoController> mSearchTeacherInfoController;

};

