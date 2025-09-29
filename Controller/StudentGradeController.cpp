#include <QCoreApplication>
#include <iostream>

#include "SearchStudentInfoController.h"
#include "Utils/ControllerUtils.h"
#include "Managers/DataManager.h"

using namespace ClassScheduler;

SearchStudentInfoController::SearchStudentInfoController(CoreFrameworkPtr coreFramework, QObject* parent)
    : mCoreFramework(coreFramework)
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
    auto coreFramework = mCoreFramework.lock();
    auto dataManager = coreFramework->getDataManager();
    if(!dataManager)
    {
        return;
    }

    if(mStudentInfosFromDB.size() == 0)
    {
        mStudentInfosFromDB = dataManager->getStudentInfosFromDB();
        mAppSettings = dataManager->getAppSettingsFromDB();
    }

    if(mStudentInfosFromDB.size() == 0)
    {
        LOG_INFO("Fail to get teacher infos from DB");
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











































