#pragma once

#include <QCoreApplication>
#include <iostream>

#include "SearchClassInfoController.h"
#include "Utils/ControllerUtils.h"
#include "Managers/DataManager.h"

using namespace ClassScheduler;

SearchClassInfoController::SearchClassInfoController(DataManagerPtr DataManager, QObject* parent)
    : mDataManager(DataManager)
    , QObject(parent)
{
}

void SearchClassInfoController::initialize()
{
    refreshSearchClassInfo();
}

void SearchClassInfoController::refreshSearchClassInfo()
{
    if(mClassInfosFromDB.size() == 0)
    {
        if(!mDataManager->isTableExist(CLASS_INFOS_TABLE_NAME))
        {
            cout << "DB is not exist" << endl;
            return;
        }

        readClassInfosFromDB();
    }

    if(mClassInfosFromDB.size() == 0)
    {
        cout << "Fail to get class infos from DB" << endl;
        return;
    }

    updateClassInfosList(mClassInfosFromDB);
}

void SearchClassInfoController::readClassInfosFromDB()
{
    if(mDataManager)
    {
        mDataManager->createDBConnection();
        mDataManager->queryDataFromClassInfosTable(mClassInfosFromDB);
    }

    if(mClassInfosFromDB.size() == 0)
    {
        cout << "Fail to read class infos from DB" << endl;
    }
}

void SearchClassInfoController::updateClassInfosList(ClassInfos& infos)
{
    QVariantMap newClassInfoMap;
    CUtils::updateClassInfoList(newClassInfoMap, infos);

    if (mClassInfoMap != newClassInfoMap)
    {
        mClassInfoMap = std::move(newClassInfoMap);
        emit classInfoMapChanged();
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











































