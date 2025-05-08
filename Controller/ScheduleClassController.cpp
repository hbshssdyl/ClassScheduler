#include "ScheduleClassController.h"

ScheduleClassController::ScheduleClassController(DataManagerPtr DataManager, QObject* parent)
    : mDataManager(DataManager)
    , QObject(parent)
{
    initialize();
}

void ScheduleClassController::initialize()
{
    if(mDataManager)
    {
        mClassInfosFromDB = mDataManager->getClassInfosFromDB();
        mTeacherInfosFromDB = mDataManager->getTeacherInfosFromDB();
    }
}

void ScheduleClassController::onRequiredInfosReceived(QVariantList requiredInfosList)
{
    for(auto& info : requiredInfosList)
    {
        cout << info.toString().toStdString() << endl;
    }
}

