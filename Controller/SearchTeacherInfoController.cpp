#pragma once

#include <QCoreApplication>
#include "SearchTeacherInfoController.h"
#include "Utils/ControllerUtils.h"

using namespace ClassScheduler;

SearchTeacherInfoController::SearchTeacherInfoController(QObject* parent)
    : QObject(parent)
{
}

void SearchTeacherInfoController::initialize(vector<teacherInfo> teacherInfos)
{
    mTeacherInfos = teacherInfos;
    refreshSearchTeacherInfo();
}

void SearchTeacherInfoController::refreshSearchTeacherInfo()
{
    QVariantList newTeacherInfoList;
    CUtils::updateTeacherInfoList(newTeacherInfoList, mTeacherInfos);

    if (mTeacherInfoList != newTeacherInfoList)
    {
        mTeacherInfoList = std::move(newTeacherInfoList);
        emit teacherInfoListChanged();
    }
}











































