#include "AccountViewController.h"
#include "Managers/AccountManager.h"
#include "Utils/ControllerUtils.h"

AccountViewController::AccountViewController(CoreFrameworkPtr& coreFramework, QObject *parent)
    : mCoreFramework(coreFramework)
    , QObject{parent}
{
    initialize();
}

void AccountViewController::initialize()
{
    refreshAccountList();
}

void AccountViewController::refreshAccountList()
{
    if(auto accountManager = mCoreFramework->getAccountManager())
    {
        mAccounts = accountManager->getUsers();
    }

    if(mAccounts.empty())
    {
        return;
    }

    QVariantList pendingAccountList;
    QVariantList finishedAccountList;
    QVariantList blacklistAccountList;
    CUtils::updateAccountsList(pendingAccountList, finishedAccountList, blacklistAccountList, mAccounts);

    if (mPendingAccountList != pendingAccountList)
    {
        mPendingAccountList = std::move(pendingAccountList);
    }

    if (mFinishedAccountList != finishedAccountList)
    {
        mFinishedAccountList = std::move(finishedAccountList);
    }

    if (mBlacklistAccountList != blacklistAccountList)
    {
        mBlacklistAccountList = std::move(blacklistAccountList);
    }
    emit accountListChanged();
}

void AccountViewController::approveAccount(int accountId)
{
    if(auto accountManager = mCoreFramework->getAccountManager())
    {
        auto result = accountManager->approveUser(accountId);
        cout << "approve account: " << result << endl;
        if(result)
        {
            refreshAccountList();
        }
    }
}

void AccountViewController::rejectAccount(int accountId)
{
    if(auto accountManager = mCoreFramework->getAccountManager())
    {
        auto result = accountManager->rejectUser(accountId);
        cout << "reject account: " << result << endl;
        if(result)
        {
            refreshAccountList();
        }
    }
}

void AccountViewController::blacklistAccount(int accountId)
{
    if(auto accountManager = mCoreFramework->getAccountManager())
    {
        auto result = accountManager->blacklistUser(accountId);
        cout << "blacklist account: " << result << endl;
        if(result)
        {
            refreshAccountList();
        }
    }
}

void AccountViewController::deleteAccount(int accountId)
{
    if(auto accountManager = mCoreFramework->getAccountManager())
    {
        auto result = accountManager->deleteUser(accountId);
        cout << "delete account: " << result << endl;
        if(result)
        {
            refreshAccountList();
        }
    }
}

void AccountViewController::addAccount(QString username, QString password, QString email)
{
    if(auto accountManager = mCoreFramework->getAccountManager())
    {
        auto result = accountManager->addUser(username.toStdString(), password.toStdString(), email.toStdString());
        cout << "add account: " << result << endl;
        if(result)
        {
            refreshAccountList();
        }
    }
}
