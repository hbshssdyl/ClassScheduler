#pragma once

#include <QCoreApplication>
#include <iostream>

#include "SearchTeacherInfoController.h"
#include "Utils/ControllerUtils.h"

using namespace ClassScheduler;

SearchTeacherInfoController::SearchTeacherInfoController(DataManagerPtr DataManager, QObject* parent)
    : mDataManager(DataManager)
    , QObject(parent)
{
    initialize();
}

void SearchTeacherInfoController::initialize()
{
    refreshSearchTeacherInfo();
}

void SearchTeacherInfoController::refreshSearchTeacherInfo()
{
    if(mTeacherInfosFromDB.size() == 0)
    {
        mTeacherInfosFromDB = mDataManager->getTeacherInfosFromDB();
    }

    if(mTeacherStudentInfosFromDB.size() == 0)
    {
        mTeacherStudentInfosFromDB = mDataManager->getTeacherStudentInfosFromDB();
    }

    mTeacherStudentBasicInfoFromDB = mDataManager->getTeacherStudentBasicInfoFromDB();

    if(mTeacherInfosFromDB.size() == 0)
    {
        cout << "Fail to get teacher infos from DB" << endl;
        return;
    }

    if(mTeacherStudentInfosFromDB.size() == 0)
    {
        cout << "Fail to get seacher student infos from DB" << endl;
        //return;
    }

    updateTeacherInfosList(mTeacherInfosFromDB, mTeacherStudentInfosFromDB, mTeacherStudentBasicInfoFromDB);
}

void SearchTeacherInfoController::updateTeacherInfosList(TeacherInfos& teacherInfos, TeacherStudentInfos& teacherStudentInfos, TeacherStudentBasicInfo& studentBasicInfo)
{
    QVariantMap newTeacherInfoMap;
    CUtils::updateTeacherInfoList(newTeacherInfoMap, teacherInfos, teacherStudentInfos, studentBasicInfo);

    if (mTeacherInfoMap != newTeacherInfoMap)
    {
        mTeacherInfoMap = std::move(newTeacherInfoMap);
        emit teacherInfoMapChanged();
    }
}

void SearchTeacherInfoController::onSearchTriggered(QString searchString)
{
    TeacherInfos infos;
    searchString = searchString.simplified();
    if(searchString.isEmpty())
    {
        infos = mTeacherInfosFromDB;
    }
    else
    {
        CUtils::doSearchTeacherInfos(mTeacherInfosFromDB, infos, searchString);
    }

    updateTeacherInfosList(infos, mTeacherStudentInfosFromDB, mTeacherStudentBasicInfoFromDB);
}











































