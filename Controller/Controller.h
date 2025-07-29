#pragma once

#include <QObject>
#include <cstdio>
#include <QString>
#include <QVariant>
#include <QPointer>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <condition_variable>

#include "Managers/DataManager.h"
#include "Managers/UserManager.h"
#include "Managers/NetworkManager.h"
#include "SearchClassInfoController.h"
#include "SearchTeacherInfoController.h"
#include "SearchStudentInfoController.h"
#include "ScheduleClassController.h"

using namespace std;

class Controller : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(OperateMode operateMode MEMBER mOperateMode NOTIFY operateModeChanged)
    Q_PROPERTY(bool showActions MEMBER mShowActions NOTIFY operateModeChanged)
    Q_PROPERTY(bool showUserInfo MEMBER mShowUserInfo NOTIFY operateModeChanged)
    Q_PROPERTY(QString name MEMBER mName NOTIFY nameChanged)
    Q_PROPERTY(QString dataCount MEMBER mDataCount NOTIFY dataCountChanged)
    Q_PROPERTY(QVariantList actionItemsList MEMBER mActionItemsList NOTIFY actionItemsListChanged)
    Q_PROPERTY(QVariantMap appSettings MEMBER mAppSettings NOTIFY appSettingsChanged)

public:
    explicit Controller(QObject* parent = nullptr);
    void initialize();

    Q_INVOKABLE virtual SearchClassInfoController* getSearchClassInfoController();
    Q_INVOKABLE virtual SearchTeacherInfoController* getSearchTeacherInfoController();
    Q_INVOKABLE virtual SearchStudentInfoController* getSearchStudentInfoController();
    Q_INVOKABLE virtual ScheduleClassController* getScheduleClassController();

public slots:
    void onOperateModeSelected(OperateMode mode);
    void onTryToLogin(QString login, QString password);
    void onTryToRegister(QString email, QString username, QString password, QString role);
    void onFileUploaded(QString filePath);

signals:
    void registerOrLoginResult(QString statusStr);
    void refreshDatabaseFinished();
    void updateOperateMode(QString mode);

signals:
    void operateModeChanged();
    void dataCountChanged();
    void actionItemsListChanged();
    void appSettingsChanged();
    void nameChanged();

private:
    QString toOperateModeString(OperateMode mode);
    void refreshOperateMode(OperateMode mode);
    void getClassInfosByExcelFile(QString filePath);
    void initManagers();
    void initDB();
    void refreshAppSettings();
    void refreshActionItems();
    void refreshAllDataFromDBFile();

private:
    OperateMode mOperateMode { OperateMode::None };
    DataManagerPtr mDataManager;
    UserManagerPtr mUserManager;
    NetworkManagerPtr mNetworkManager;

    QString mNewDataFilePath { "" };
    UserInfo mUserInfo;

    QString mName{ "" };
    bool mShowActions;
    bool mShowUserInfo;
    QString mDataCount { "" };
    QVariantList mActionItemsList;
    OperateModes mAllOperateMode;
    QVariantMap mAppSettings;

    atomic_bool mAllDataIsReady = false;

    QPointer<SearchClassInfoController> mSearchClassInfoController;
    QPointer<SearchTeacherInfoController> mSearchTeacherInfoController;
    QPointer<SearchStudentInfoController> mSearchStudentInfoController;
    QPointer<ScheduleClassController> mScheduleClassController;

    QFutureWatcher<void> mFutureWatcher; // 跟踪异步任务
    std::mutex mClassInfosMutex;
    std::condition_variable mClassInfosCondition;
};

