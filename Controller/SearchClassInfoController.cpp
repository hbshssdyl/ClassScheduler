#include <QCoreApplication>
#include <iostream>

#include "SearchClassInfoController.h"
#include "Utils/ControllerUtils.h"
#include "Managers/DataManager.h"

using namespace ClassScheduler;

SearchClassInfoController::SearchClassInfoController(CoreFrameworkPtr coreFramework, QObject* parent)
    : mCoreFramework(coreFramework)
    , QObject(parent)
{
    initialize();
}

void SearchClassInfoController::initialize()
{
    refreshSearchClassInfo();
}

void SearchClassInfoController::refreshSearchClassInfo()
{
    auto coreFramework = mCoreFramework.lock();
    auto dataManager = coreFramework->getDataManager();
    if(!dataManager)
    {
        return;
    }

    if(mClassInfosFromDB.size() == 0)
    {
        mClassInfosFromDB = dataManager->getClassInfosFromDB();
    }

    if(mClassInfosFromDB.size() == 0)
    {
        cout << "Fail to get class infos from DB" << endl;
        return;
    }

    updateClassInfosList(mClassInfosFromDB);
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











































