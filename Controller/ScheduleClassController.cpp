#include "ScheduleClassController.h"
#include "Utils/ControllerUtils.h"
#include "Managers/DataManager.h"

ScheduleClassController::ScheduleClassController(CoreFrameworkPtr coreFramework, QObject* parent)
    : mCoreFramework(coreFramework)
    , QObject(parent)
{
    initialize();
}

void ScheduleClassController::initialize()
{
    auto coreFramework = mCoreFramework.lock();
    if(auto dataManager = coreFramework->getDataManager())
    {
        mClassInfosFromDB = dataManager->getClassInfosFromDB();
        mTeacherInfosFromDB = dataManager->getTeacherInfosFromDB();
    }
}

void ScheduleClassController::onRequiredInfosReceived(QVariantList requiredInfosList)
{
    //科目，年级，星期，开始时间，结束时间
    // requiredInfosList.clear();
    // requiredInfosList.append("物理");
    // requiredInfosList.append("高三");
    // requiredInfosList.append("星期六");
    // requiredInfosList.append("15：00");
    // requiredInfosList.append("17：00");
    if (mRequiredInfosList == requiredInfosList)
    {
        return;
    }
    mRequiredInfosList = std::move(requiredInfosList);

    ScheduleClassInputInfo inputInfos(mRequiredInfosList);
    updateScheduleClassResultsList(inputInfos);
}

void ScheduleClassController::updateScheduleClassResultsList(ScheduleClassInputInfo inputInfos)
{
    auto coreFramework = mCoreFramework.lock();
    auto dataManager = coreFramework->getDataManager();
    if(!dataManager)
    {
        return;
    }

    auto classInfosList = dataManager->getClassInfosFromDB();
    if(classInfosList.size() == 0)
    {
        return;
    }

    QList<QList<QVariantMap>> newScheduleClassResultsList;
    CUtils::updateScheduleClassResultsList(newScheduleClassResultsList, inputInfos, classInfosList);

    if (mScheduleClassResultsList != newScheduleClassResultsList)
    {
        mScheduleClassResultsList = std::move(newScheduleClassResultsList);
        emit scheduleClassResultsListChanged();
    }
}

