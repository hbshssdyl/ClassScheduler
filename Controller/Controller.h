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
#include "SearchStudentInfoController.h"
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
    Q_PROPERTY(QVariantMap appSettings MEMBER mAppSettings NOTIFY appSettingsChanged)

    enum class OperateMode
    {
        None,
        LoginView, //once view
        FileView,  //once view
        WelcomePage,
        SearchClassInfo,
        SearchTeacherInfo,
        SearchStudentInfo,
        ScheduleClass,
        TeacherEvaluation
    };
    Q_ENUM(OperateMode)
    using OperateModes = std::vector<OperateMode>;

public:
    explicit Controller(QObject* parent = nullptr);
    void initialize();

    Q_INVOKABLE virtual SearchClassInfoController* getSearchClassInfoController();
    Q_INVOKABLE virtual SearchTeacherInfoController* getSearchTeacherInfoController();
    Q_INVOKABLE virtual SearchStudentInfoController* getSearchStudentInfoController();
    Q_INVOKABLE virtual ScheduleClassController* getScheduleClassController();

public slots:
    void onOperateModeSelected(OperateMode mode);
    void onTryToLogin(QString username, QString password);
    void onFileUploaded(QString filePath);

signals:
    void operateModeChanged();
    void dataCountChanged();
    void actionItemsListChanged();
    void appSettingsChanged();

private:
    QString toOperateModeString(OperateMode mode);
    void refreshOperateMode(OperateMode mode);
    void getClassInfosByExcelFile(QString filePath);
    void initDB();
    void refreshAppSettings();

private:
    OperateMode mOperateMode { OperateMode::None };
    DataManagerPtr mDataManager;

    QString mNewDataFilePath { "" };

    QString mLoadedView { "" };
    bool mShowActions;
    QString mDataCount { "" };
    QVariantList mActionItemsList;
    OperateModes mAllOperateMode;
    QVariantMap mAppSettings;

    QPointer<SearchClassInfoController> mSearchClassInfoController;
    QPointer<SearchTeacherInfoController> mSearchTeacherInfoController;
    QPointer<SearchStudentInfoController> mSearchStudentInfoController;
    QPointer<ScheduleClassController> mScheduleClassController;

    QFutureWatcher<void> mFutureWatcher; // 跟踪异步任务
    std::mutex mClassInfosMutex;
    std::condition_variable mClassInfosCondition;
};

