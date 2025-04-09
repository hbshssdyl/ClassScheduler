#pragma once

#include "SearchTeacherInfoController.h"
#include "Utils/ControllerUtils.h"

using namespace ClassScheduler;

SearchTeacherInfoController::SearchTeacherInfoController(QObject* parent)
{
    cout<<"mytest"<<endl;
    //initialize();
}

void SearchTeacherInfoController::initialize()
{
    refreshSearchTeacherInfo();
}

void SearchTeacherInfoController::refreshSearchTeacherInfo()
{
    QVariantList newTeacherInfoList;
    CUtils::updateTeacherInfoList(newTeacherInfoList);

    if (mTeacherInfoList != newTeacherInfoList)
    {
        mTeacherInfoList = std::move(newTeacherInfoList);
        emit teacherInfoListChanged();
    }
    cout<<"mytest7"<<endl;
}











































