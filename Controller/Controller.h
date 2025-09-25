#pragma once

#include <QObject>
#include <cstdio>
#include <QString>
#include <QVariant>
#include <QPointer>
#include <QProcess>  // 新增：用于启动进程

#include "Managers/CoreFramework.h"
#include "SearchClassInfoController.h"
#include "SearchTeacherInfoController.h"
#include "SearchStudentInfoController.h"
#include "ScheduleClassController.h"
#include "DatabaseController.h"
#include "TaskController.h"
#include "AccountViewController.h"
#include "FeedbackController.h"

using namespace std;

class Controller : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(OperateMode operateMode MEMBER mOperateMode NOTIFY operateModeChanged)
    Q_PROPERTY(bool showActions MEMBER mShowActions NOTIFY operateModeChanged)
    Q_PROPERTY(bool showUserInfo MEMBER mShowUserInfo NOTIFY operateModeChanged)
    Q_PROPERTY(bool refreshFinished MEMBER mRefreshFinished NOTIFY refreshFinishedChanged)
    Q_PROPERTY(QString name MEMBER mName NOTIFY nameChanged)
    Q_PROPERTY(int downloadValue MEMBER mDownloadValue NOTIFY downloadValueChanged)
    Q_PROPERTY(QVariantList actionItemsList MEMBER mActionItemsList NOTIFY actionItemsListChanged)
    Q_PROPERTY(QVariantMap appSettings MEMBER mAppSettings NOTIFY appSettingsChanged)
    Q_PROPERTY(QVariantMap versionInfoList MEMBER mVersionInfoList NOTIFY versionInfoListChanged)

public:
    explicit Controller(QObject* parent = nullptr);
    Q_INVOKABLE void init();

    Q_INVOKABLE virtual SearchClassInfoController* getSearchClassInfoController();
    Q_INVOKABLE virtual SearchTeacherInfoController* getSearchTeacherInfoController();
    Q_INVOKABLE virtual SearchStudentInfoController* getSearchStudentInfoController();
    Q_INVOKABLE virtual ScheduleClassController* getScheduleClassController();
    Q_INVOKABLE virtual DatabaseController* getDatabaseController();
    Q_INVOKABLE virtual TaskController* getTaskController();
    Q_INVOKABLE virtual AccountViewController* getAccountViewController();
    Q_INVOKABLE virtual FeedbackController* getFeedbackController();

public slots:
    void onOperateModeSelected(OperateMode mode);
    void onTryToLogin(QString login, QString password);
    void onTryToRegister(QString email, QString username, QString password, QString role);
    void onUpdateStarted();
    void installLatestVersion();
    void updateAllData();

signals:
    void registerOrLoginResult(QString statusStr);
    void updateOperateMode(QString mode);
    void operateModeChanged();
    void actionItemsListChanged();
    void appSettingsChanged();
    void nameChanged();
    void versionInfoListChanged();
    void downloadValueChanged();
    void dataUpdateFinished();
    void refreshFinishedChanged();

private:
    QString toOperateModeString(OperateMode mode);
    void refreshOperateMode(OperateMode mode);
    void getClassInfosByExcelFile(QString filePath);
    void initCoreFramework();
    void refreshActionItems();
    void refreshControllersData();
    void refreshLatestVersionInfo();
    void onInstallFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    OperateMode mOperateMode { OperateMode::None };
    CoreFrameworkPtr mCoreFramework;

    UserInfo mUserInfo;

    QString mName{ "" };
    bool mShowActions{ false };
    bool mShowUserInfo{ false };
    bool mRefreshFinished{ false };
    int mDownloadValue{ 0 };
    QString mDataCount { "" };
    QVariantList mActionItemsList;
    QVariantMap mVersionInfoList;
    OperateModes mAllOperateMode;
    QVariantMap mAppSettings;

    atomic_bool mAllDataIsReady = false;
    QProcess *m_installProcess = nullptr;  // 新增：进程对象

    QPointer<SearchClassInfoController> mSearchClassInfoController;
    QPointer<SearchTeacherInfoController> mSearchTeacherInfoController;
    QPointer<SearchStudentInfoController> mSearchStudentInfoController;
    QPointer<ScheduleClassController> mScheduleClassController;
    QPointer<DatabaseController> mDatabaseController;
    QPointer<TaskController> mTaskController;
    QPointer<AccountViewController> mAccountViewController;
    QPointer<FeedbackController> mFeedbackController;
};
