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
    CUtils::updateAccountsList(pendingAccountList, finishedAccountList, mAccounts);

    if (mPendingAccountList != pendingAccountList)
    {
        mPendingAccountList = std::move(pendingAccountList);
    }

    if (mFinishedAccountList != finishedAccountList)
    {
        mFinishedAccountList = std::move(finishedAccountList);
    }
    emit accountListChanged();
}
