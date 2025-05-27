#pragma once

#include <QObject>
#include <cstdio>
#include <vector>
#include <QString>
#include <QVariant>
#include <QPointer>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <condition_variable>

#include "Managers/DataManager.h"
#include "SearchClassInfoController.h"
#include "SearchTeacherInfoController.h"
#include "ScheduleClassController.h"

using namespace std;

class Controller : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(OperateMode operateMode MEMBER mOperateMode NOTIFY operateModeChanged)
    Q_PROPERTY(bool showActions MEMBER mShowActions NOTIFY operateModeChanged)
    Q_PROPERTY(QString loadedView MEMBER mLoadedView NOTIFY operateModeChanged)
    Q_PROPERTY(QString dataCount MEMBER mDataCount NOTIFY dataCountChanged)
    Q_PROPERTY(QVariantList actionItemsList MEMBER mActionItemsList NOTIFY actionItemsListChanged)

    enum class OperateMode
    {
        None,
        LoginView, //once view
        FileView,  //once view
        WelcomePage,
        SearchClassInfo,
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

    Q_INVOKABLE virtual SearchClassInfoController* getSearchClassInfoController();
    Q_INVOKABLE virtual SearchTeacherInfoController* getSearchTeacherInfoController();
    Q_INVOKABLE virtual ScheduleClassController* getScheduleClassController();

public slots:
    void onOperateModeSelected(OperateMode mode);
    void onTryToLogin(QString username, QString password);
    void onFileUploaded(QString filePath);

signals:
    void operateModeChanged();
    void dataCountChanged();
    void actionItemsListChanged();

private:
    QString toOperateModeString(OperateMode mode);
    void refreshOperateMode(OperateMode mode);
    void getClassInfosByExcelFile(QString filePath);
    void initDB();

private:
    OperateMode mOperateMode { OperateMode::None };
    DataManagerPtr mDataManager;
    QFutureWatcher<void> mFutureWatcher; // 跟踪异步任务
    QString mLoadedView { "" };
    bool mShowActions;
    QString mDataCount { "" };
    QString mNewDataFilePath { "" };
    QVariantList mActionItemsList;
    OperateModes mAllOperateMode;
    QPointer<SearchClassInfoController> mSearchClassInfoController;
    QPointer<SearchTeacherInfoController> mSearchTeacherInfoController;
    QPointer<ScheduleClassController> mScheduleClassController;

    std::mutex mClassInfosMutex;
    std::condition_variable mClassInfosCondition;

};

