#pragma once

#include <QObject>
#include <cstdio>
#include <vector>
#include <QString>
#include <QVariant>
#include <QPointer>
#include <condition_variable>

#include "Managers/DBManager.h"
#include "SearchTeacherInfoController.h"

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
        LoginView,
        FileView,
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
    void onTryToLogin(QString username, QString password);
    void onFileUploaded(QString filePath);

signals:
    void operateModeChanged();
    void dataCountChanged();
    void actionItemsListChanged();

private:
    QString toOperateModeString(OperateMode mode);
    void refreshOperateMode(OperateMode mode);
    void getTeacherInfosByExcelFile(QString filePath);
    void initDB();

private:
    OperateMode mOperateMode { OperateMode::None };
    DBManagerPtr mDBManager;
    QString mLoadedView { "" };
    bool mShowActions;
    QString mDataCount { "" };
    QString mNewDataFilePath { "" };
    QVariantList mActionItemsList;
    OperateModes mAllOperateMode;
    QPointer<SearchTeacherInfoController> mSearchTeacherInfoController;

    std::mutex mTeacherInfosMutex;
    std::condition_variable mTeacherInfosCondition;

};

