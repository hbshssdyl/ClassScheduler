#include <QCoreApplication>
#include <iostream>

#include "SearchTeacherInfoController.h"
#include "Utils/ControllerUtils.h"
#include "Managers/DataManager.h"

using namespace ClassScheduler;

SearchTeacherInfoController::SearchTeacherInfoController(CoreFrameworkPtr coreFramework, QObject* parent)
    : mCoreFramework(coreFramework)
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
    auto coreFramework = mCoreFramework.lock();
    auto dataManager = coreFramework->getDataManager();
    if(!dataManager)
    {
        return;
    }

    if(mTeacherInfosFromDB.size() == 0)
    {
        mTeacherInfosFromDB = dataManager->getTeacherInfosFromDB();
    }

    if(mTeacherInfosFromDB.size() == 0)
    {
        LOG_INFO("Fail to get teacher infos from DB");
    }

    updateTeacherInfosList(mTeacherInfosFromDB);
}

void SearchTeacherInfoController::updateTeacherInfosList(TeacherInfos& teacherInfos)
{
    QVariantMap newTeacherInfoMap;
    CUtils::updateTeacherInfoList(newTeacherInfoMap, teacherInfos);

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











































