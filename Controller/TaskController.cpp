#include "TaskController.h"

TaskController::TaskController(CoreFrameworkPtr& coreFramework, QObject *parent)
    : mCoreFramework(coreFramework)
    , QObject{parent}
{
    initialize();
}

void TaskController::initialize()
{
    refreshTaskList();
}

void TaskController::refreshTaskList()
{
    if(auto taskManager = mCoreFramework->getTaskManager())
    {
        mTasks = taskManager->getTasks();
    }
}
