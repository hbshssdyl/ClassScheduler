#pragma once

#include "Controller.h"
#include "Utils/ControllerUtils.h"
#include "SearchTeacherInfoController.h"

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
    onOperateModeSelected(OperateMode::WelcomePage);
}

std::string Controller::toOperateModeString(OperateMode mode)
{
    switch (mode)
    {
    case OperateMode::None:
        return "None";
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
    mLoadedView = QString::fromStdString(toOperateModeString(mode));
    emit operateModeChanged();
}

void Controller::onOperateModeSelected(OperateMode mode)
{
    cout << toOperateModeString(mode) << endl;
    refreshOperateMode(mode);

    QVariantList newActionItemsList;
    CUtils::updateActionItemsList(newActionItemsList, mOperateMode, mAllOperateMode);

    if (mActionItemsList != newActionItemsList)
    {
        mActionItemsList = std::move(newActionItemsList);
        emit actionItemsListChanged();
    }
}

SearchTeacherInfoController* Controller::getSearchTeacherInfoController()
{
    if (!mSearchTeacherInfoController)
    {
        cout<<"mytest1"<<endl;
        mSearchTeacherInfoController = new SearchTeacherInfoController(this);
        cout<<"mytest2"<<endl;
        mSearchTeacherInfoController->initialize();
        cout<<"mytest3"<<endl;
    }
    return mSearchTeacherInfoController;
}













































