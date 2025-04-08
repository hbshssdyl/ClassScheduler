#pragma once

#include "SearchTeacherInfoController.h"
#include "Utils/ControllerUtils.h"

SearchTeacherInfoController::SearchTeacherInfoController(QObject* parent)
{
    refreshOperateMode(OperateMode::WelcomePage);
    initialize();
}

void SearchTeacherInfoController::initialize()
{
    mAllOperateMode.clear();
    mAllOperateMode.emplace_back(OperateMode::SearchTeacherInfo);
    mAllOperateMode.emplace_back(OperateMode::ScheduleClass);
    mAllOperateMode.emplace_back(OperateMode::CalcOneToOneMoney);
    mAllOperateMode.emplace_back(OperateMode::CalcClassMoney);
    onOperateModeSelected(OperateMode::WelcomePage);
}













































