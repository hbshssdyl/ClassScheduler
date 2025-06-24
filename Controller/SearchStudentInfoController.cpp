#pragma once

#include <QCoreApplication>
#include <iostream>

#include "SearchStudentInfoController.h"
#include "Utils/ControllerUtils.h"

using namespace ClassScheduler;

SearchStudentInfoController::SearchStudentInfoController(DataManagerPtr DataManager, QObject* parent)
    : mDataManager(DataManager)
    , QObject(parent)
{
    initialize();
}

void SearchStudentInfoController::initialize()
{
    refreshSearchStudentInfo();
}

void SearchStudentInfoController::refreshSearchStudentInfo()
{
    if(mStudentInfosFromDB.size() == 0)
    {
        mStudentInfosFromDB = mDataManager->getStudentInfosFromDB();
        mAppSettings = mDataManager->getAppSettingsFromDB();
    }

    if(mStudentInfosFromDB.size() == 0)
    {
        cout << "Fail to get teacher infos from DB" << endl;
    }

    updateStudentInfosList(mStudentInfosFromDB);
}

void SearchStudentInfoController::updateStudentInfosList(StudentInfos& studentInfos)
{
    QVariantMap newStudentInfoMap;
    CUtils::updateStudentInfoList(newStudentInfoMap, studentInfos);

    if (mStudentInfoMap != newStudentInfoMap)
    {
        mStudentInfoMap = std::move(newStudentInfoMap);
        emit studentInfoMapChanged();
    }
}

void SearchStudentInfoController::onSearchTriggered(QString searchString)
{
    StudentInfos infos;
    searchString = searchString.simplified();
    if(searchString.isEmpty())
    {
        infos = mStudentInfosFromDB;
    }
    else
    {
        CUtils::doSearchStudentInfos(mStudentInfosFromDB, infos, searchString);
    }

    updateStudentInfosList(infos);
}











































