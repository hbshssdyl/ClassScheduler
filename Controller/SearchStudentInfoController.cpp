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
    }

    // if(mTeacherStudentInfosFromDB.size() == 0)
    // {
    //     mTeacherStudentInfosFromDB = mDataManager->getTeacherStudentInfosFromDB();
    // }

    // mTeacherStudentBasicInfoFromDB = mDataManager->getTeacherStudentBasicInfoFromDB();

    if(mStudentInfosFromDB.size() == 0)
    {
        cout << "Fail to get teacher infos from DB" << endl;
    }

    // if(mTeacherStudentInfosFromDB.size() == 0)
    // {
    //     cout << "Fail to get seacher student infos from DB" << endl;
    // }

    updateStudentInfosList(mStudentInfosFromDB/*, mTeacherStudentInfosFromDB, mTeacherStudentBasicInfoFromDB*/);
}

void SearchStudentInfoController::updateStudentInfosList(StudentInfos& studentInfos/*, TeacherStudentInfos& teacherStudentInfos, TeacherStudentBasicInfo& studentBasicInfo*/)
{
    QVariantMap newStudentInfoMap;
    CUtils::updateStudentInfoList(newStudentInfoMap, studentInfos/*, teacherStudentInfos, studentBasicInfo*/);

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

    updateStudentInfosList(infos/*, mTeacherStudentInfosFromDB, mTeacherStudentBasicInfoFromDB*/);
}











































