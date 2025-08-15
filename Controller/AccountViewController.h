#ifndef ACCOUNTVIEWCONTROLLER_H
#define ACCOUNTVIEWCONTROLLER_H

#include <QObject>
#include "Managers/CoreFramework.h"

class AccountViewController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList pendingAccountList MEMBER mPendingAccountList NOTIFY accountListChanged)
    Q_PROPERTY(QVariantList finishedAccountList MEMBER mFinishedAccountList NOTIFY accountListChanged)
    Q_PROPERTY(QVariantList blacklistAccountList MEMBER mBlacklistAccountList NOTIFY accountListChanged)

public:
    explicit AccountViewController(CoreFrameworkPtr& coreFramework, QObject *parent = nullptr);

private:
    void initialize();
    void refreshAccountList();

public slots:
    void approveAccount(int accountId);
    void rejectAccount(int accountId);
    void blacklistAccount(int accountId);
    void deleteAccount(int accountId);
    void addAccount(QString username, QString password, QString email);

signals:
    void accountListChanged();

private:
    CoreFrameworkPtr mCoreFramework;
    QVariantList mPendingAccountList;
    QVariantList mFinishedAccountList;
    QVariantList mBlacklistAccountList;
    UserInfos mAccounts;
};

#endif // ACCOUNTVIEWCONTROLLER_H
