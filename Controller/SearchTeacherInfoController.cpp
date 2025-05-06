#pragma once

#include <QCoreApplication>
#include <iostream>

#include "SearchTeacherInfoController.h"
#include "Utils/ControllerUtils.h"

using namespace ClassScheduler;

SearchTeacherInfoController::SearchTeacherInfoController(DBManagerPtr dbManager, QObject* parent)
    : mDBManager(dbManager)
    , QObject(parent)
{
}

void SearchTeacherInfoController::initialize()
{
    refreshSearchTeacherInfo();
}

void SearchTeacherInfoController::refreshSearchTeacherInfo()
{
    if(!mDBManager->isTableExist(CLASS_INFOS_TABLE_NAME))
    {
        cout << "DB is not exist" << endl;
        return;
    }

    initTeahcerHeader();
    readTeacherInfosFromDB();

    if(mTeacherInfosFromDB.size() == 0)
    {
        cout << "Fail to get class infos from DB" << endl;
        return;
    }

    updateTeacherInfosList(mTeacherInfosFromDB);
}

void SearchTeacherInfoController::initTeahcerHeader()
{
    QVariantList newTeacherrHeaderList;
    CUtils::updateTeacherHeaderList(newTeacherrHeaderList);

    if (mTeacherHeaderList != newTeacherrHeaderList)
    {
        mTeacherHeaderList = std::move(newTeacherrHeaderList);
        emit teacherInfoHeaderChanged();
    }
}

void SearchTeacherInfoController::readTeacherInfosFromDB()
{
    // if(mDBManager)
    // {
    //     mDBManager->createDBConnection();
    //     mDBManager->queryDataFromTeacherInfosTable(mTeacherInfosFromDB);
    // }

    if(mTeacherInfosFromDB.size() == 0)
    {
        cout << "Fail to read class infos from DB" << endl;
    }
}

void SearchTeacherInfoController::updateTeacherInfosList(TeacherInfos& infos)
{
    QVariantList newTeacherInfoList;
    CUtils::updateTeacherInfoList(newTeacherInfoList, infos);

    if (mTeacherInfoList != newTeacherInfoList)
    {
        mTeacherInfoList = std::move(newTeacherInfoList);
        emit teacherInfoListChanged();
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











































