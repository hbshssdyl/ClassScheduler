#include "TaskController.h"
#include "Managers/TaskManager.h"
#include "Utils/ControllerUtils.h"

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

    if(mTasks.empty())
    {
        return;
    }

    QVariantList taskList;
    CUtils::updateTasksList(taskList, mTasks);

    if (mTaskList != taskList)
    {
        mTaskList = std::move(taskList);
        emit taskListChanged();
    }
}

void TaskController::onTaskFinished(int taskId, QString commentString)
{
    if(auto taskManager = mCoreFramework->getTaskManager())
    {
        auto result = taskManager->updateTaskFinishStatus(taskId, commentString.toStdString());
        std::cout << "Update task finish status: " << result << endl;
        if(result)
        {
            refreshTaskList();
        }
    }
}
