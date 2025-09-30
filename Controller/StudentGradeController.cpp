#include <QCoreApplication>
#include <iostream>

#include "StudentGradeController.h"
#include "Utils/ControllerUtils.h"
#include "Managers/StudentGradeManager.h"

using namespace ClassScheduler;

StudentGradeController::StudentGradeController(CoreFrameworkPtr coreFramework, QObject* parent)
    : mCoreFramework(coreFramework)
    , QObject(parent)
{
    initialize();
}

void StudentGradeController::initialize()
{
    refreshStudentGradeInfos();
}

void StudentGradeController::refreshStudentGradeInfos()
{
    auto coreFramework = mCoreFramework.lock();
    auto studentGradeManager = coreFramework->getStudentGradeManager();
    if(!studentGradeManager)
    {
        return;
    }

    if(mStudentGradeInfos.size() == 0)
    {
        mStudentGradeInfos = studentGradeManager->getStudentInfosFromDB();
        mAppSettings = studentGradeManager->getAppSettingsFromDB();
    }

    if(mStudentInfosFromDB.size() == 0)
    {
        LOG_INFO("Fail to get teacher infos from DB");
    }

    updateStudentInfosList(mStudentInfosFromDB);
}

void StudentGradeController::updateStudentInfosList(StudentInfos& studentInfos)
{
    QVariantMap newStudentInfoMap;
    CUtils::updateStudentInfoList(newStudentInfoMap, studentInfos);

    if (mStudentInfoMap != newStudentInfoMap)
    {
        mStudentInfoMap = std::move(newStudentInfoMap);
        emit studentInfoMapChanged();
    }
}

void StudentGradeController::onSearchTriggered(QString searchString)
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











































