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
        LOG_INFO("approve account: " +result);
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
        LOG_INFO("reject account: " +result);
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
        LOG_INFO("blacklist account: " +result);
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
        LOG_INFO("delete account: " +result);
        if(result)
        {
            refreshAccountList();
        }
    }
}

void AccountViewController::addAccount(QString username, QString password, QString email, QString role)
{
    if(auto accountManager = mCoreFramework->getAccountManager())
    {
        auto result = accountManager->addUser(username.toStdString(), password.toStdString(), email.toStdString(), role.toStdString());
        LOG_INFO("add account: " +result);
        if(result)
        {
            refreshAccountList();
        }
    }
}
