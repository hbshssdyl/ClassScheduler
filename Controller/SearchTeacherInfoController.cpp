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
}

void SearchTeacherInfoController::initialize()
{
    refreshSearchTeacherInfo();
}

void SearchTeacherInfoController::refreshSearchTeacherInfo()
{
    if(mTeacherInfosFromDB.size() == 0)
    {
        if(!mDataManager->isTableExist(TEACHER_INFOS_TABLE_NAME))
        {
            cout << "DB is not exist" << endl;
            return;
        }

        readTeacherInfosFromDB();
    }

    if(mTeacherInfosFromDB.size() == 0)
    {
        cout << "Fail to get class infos from DB" << endl;
        return;
    }

    updateTeacherInfosList(mTeacherInfosFromDB);
}

void SearchTeacherInfoController::readTeacherInfosFromDB()
{
    if(mDataManager)
    {
        mDataManager->createDBConnection();
        mDataManager->queryDataFromTeacherInfosTable(mTeacherInfosFromDB);
    }

    if(mTeacherInfosFromDB.size() == 0)
    {
        cout << "Fail to read class infos from DB" << endl;
    }
}

void SearchTeacherInfoController::updateTeacherInfosList(TeacherInfos& infos)
{
    QVariantMap newTeacherInfoMap;
    CUtils::updateTeacherInfoList(newTeacherInfoMap, infos);

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

    updateTeacherInfosList(infos);
}











































