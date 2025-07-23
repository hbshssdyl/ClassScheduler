#include <QCoreApplication>
#include <iostream>

#include "SearchTeacherInfoController.h"
#include "Utils/ControllerUtils.h"

using namespace ClassScheduler;

SearchTeacherInfoController::SearchTeacherInfoController(DataManagerPtr DataManager, QObject* parent)
    : mDataManager(DataManager)
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
    if(mTeacherInfosFromDB.size() == 0)
    {
        mTeacherInfosFromDB = mDataManager->getTeacherInfosFromDB();
    }

    if(mTeacherInfosFromDB.size() == 0)
    {
        cout << "Fail to get teacher infos from DB" << endl;
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











































