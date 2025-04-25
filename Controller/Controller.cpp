#pragma once

#include <iostream>
#include "Controller.h"
#include "Utils/ControllerUtils.h"

using namespace ClassScheduler;

Controller::Controller(QObject* parent)
{
    refreshOperateMode(OperateMode::WelcomePage);
    initialize();
}

void Controller::initialize()
{
    mAllOperateMode.clear();
    mAllOperateMode.emplace_back(OperateMode::SearchTeacherInfo);
    mAllOperateMode.emplace_back(OperateMode::ScheduleClass);
    mAllOperateMode.emplace_back(OperateMode::CalcOneToOneMoney);
    mAllOperateMode.emplace_back(OperateMode::CalcClassMoney);
    onOperateModeSelected(OperateMode::LoginView);
    initDB();
}

void Controller::initDB()
{
    mDBManager = std::make_shared<DBManager>();
    if(mDBManager)
    {
        mDBManager->createDBConnection();
    }
}

QString Controller::toOperateModeString(OperateMode mode)
{
    switch (mode)
    {
    case OperateMode::None:
        return "None";
    case OperateMode::LoginView:
        return "LoginView";
    case OperateMode::WelcomePage:
        return "WelcomePage";
    case OperateMode::SearchTeacherInfo:
        return "SearchTeacherInfo";
    case OperateMode::ScheduleClass:
        return "ScheduleClass";
    case OperateMode::CalcOneToOneMoney:
        return "CalcOneToOneMoney";
    case OperateMode::CalcClassMoney:
        return "CalcClassMoney";
    default:
        return "default";
    }

}

void Controller::refreshOperateMode(OperateMode mode)
{
    mOperateMode = mode;
    mLoadedView = toOperateModeString(mode);
    mShowActions = mode != OperateMode::LoginView;
    emit operateModeChanged();
}

void Controller::onOperateModeSelected(OperateMode mode)
{
    cout << toOperateModeString(mode).toStdString() << endl;
    refreshOperateMode(mode);

    QVariantList newActionItemsList;
    CUtils::updateActionItemsList(newActionItemsList, mOperateMode, mAllOperateMode);

    if (mActionItemsList != newActionItemsList)
    {
        mActionItemsList = std::move(newActionItemsList);
        emit actionItemsListChanged();
    }
}

void Controller::waitTeacherInfosInited()
{
    // std::unique_lock<std::mutex> u_lk(mTeacherInfosMutex);
    // int timeout = 5;

    // if (mTeacherInfos.size() > 0)
    // {
    //     return;
    // }
    // std::cv_status ret = mTeacherInfosCondition.wait_for(u_lk, std::chrono::seconds(timeout));
    // if (ret == std::cv_status::timeout)
    // {
    //     cout << "Maybe there is something wrong with the mTeacherInfos" << endl;
    // }
    // else if (ret == std::cv_status::no_timeout)
    // {
    //     cout << "mTeacherInfos Initialized" << endl;
    // }
    // cout << "mytest waiting stop" << endl;
}

SearchTeacherInfoController* Controller::getSearchTeacherInfoController()
{
    if (!mSearchTeacherInfoController)
    {
        mSearchTeacherInfoController = new SearchTeacherInfoController(mDBManager, this);
        //waitTeacherInfosInited();
        mSearchTeacherInfoController->initialize();
    }
    return mSearchTeacherInfoController;
}













































