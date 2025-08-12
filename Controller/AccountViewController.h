#ifndef ACCOUNTVIEWCONTROLLER_H
#define ACCOUNTVIEWCONTROLLER_H

#include <QObject>
#include "Managers/CoreFramework.h"

class AccountViewController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList pendingAccountList MEMBER mPendingAccountList NOTIFY accountListChanged)
    Q_PROPERTY(QVariantList finishedAccountList MEMBER mFinishedAccountList NOTIFY accountListChanged)

public:
    explicit AccountViewController(CoreFrameworkPtr& coreFramework, QObject *parent = nullptr);

private:
    void initialize();
    void refreshAccountList();

// public slots:
//     void onTaskFinished(int taskId, QString commentString);
//     void onTaskReviewed(int taskId, QString resultRating, QString reviewString);
//     void onTaskAdded(QVariantMap task);

signals:
    void accountListChanged();

private:
    CoreFrameworkPtr mCoreFramework;
    QVariantList mPendingAccountList;
    QVariantList mFinishedAccountList;
    UserInfos mAccounts;
};

#endif // ACCOUNTVIEWCONTROLLER_H
