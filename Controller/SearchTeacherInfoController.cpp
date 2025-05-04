#pragma once

#include <QCoreApplication>
#include <thread>
#include <iostream>

#include "SearchClassInfoController.h"
#include "Utils/ControllerUtils.h"
#include "Managers/DBManager.h"

using namespace ClassScheduler;

SearchClassInfoController::SearchClassInfoController(DBManagerPtr dbManager, QObject* parent)
    : mDBManager(dbManager)
    , QObject(parent)
{
}

void SearchClassInfoController::initialize()
{
    refreshSearchClassInfo();
}

void SearchClassInfoController::refreshSearchClassInfo()
{
    if(!mDBManager->isTableExist(TEACHER_INFOS_TABLE_NAME))
    {
        cout << "DB is not exist" << endl;
        return;
    }

    initTeahcerHeader();
    readClassInfosFromDB();

    if(mClassInfosFromDB.size() == 0)
    {
        cout << "Fail to get class infos from DB" << endl;
        return;
    }

    updateClassInfosList(mClassInfosFromDB);
}

void SearchClassInfoController::initTeahcerHeader()
{
    QVariantList newTeacherrHeaderList;
    CUtils::updateTeacherHeaderList(newTeacherrHeaderList);

    if (mTeacherHeaderList != newTeacherrHeaderList)
    {
        mTeacherHeaderList = std::move(newTeacherrHeaderList);
        emit classInfoHeaderChanged();
    }
}

void SearchClassInfoController::readClassInfosFromDB()
{
    if(mDBManager)
    {
        mDBManager->createDBConnection();
        mDBManager->queryDataFromClassInfosTable(mClassInfosFromDB);
    }

    if(mClassInfosFromDB.size() == 0)
    {
        cout << "Fail to read class infos from DB" << endl;
    }
}

void SearchClassInfoController::updateClassInfosList(ClassInfos& infos)
{
    QVariantList newClassInfoList;
    CUtils::updateClassInfoList(newClassInfoList, infos);

    if (mClassInfoList != newClassInfoList)
    {
        mClassInfoList = std::move(newClassInfoList);
        emit classInfoListChanged();
    }
}

void SearchClassInfoController::onSearchTriggered(QString searchString)
{
    ClassInfos infos;
    searchString = searchString.simplified();
    if(searchString.isEmpty())
    {
        infos = mClassInfosFromDB;
    }
    else
    {
        CUtils::doSearchClassInfos(mClassInfosFromDB, infos, searchString);
    }

    updateClassInfosList(infos);
}











































