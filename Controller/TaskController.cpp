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
        LOG_INFO("Update task finish status: " +result);
        if(result)
        {
            refreshTaskList();
        }
    }
}

void TaskController::onTaskReviewed(int taskId, QString resultRating, QString reviewString)
{
    if(auto taskManager = mCoreFramework->getTaskManager())
    {
        auto result = taskManager->updateTaskReviewStatus(taskId, resultRating.toStdString(), reviewString.toStdString());
        LOG_INFO("Update task finish status: " +result);
        if(result)
        {
            refreshTaskList();
        }
    }
}

void TaskController::onTaskAdded(QVariantMap taskMap)
{
    Task task;

    task.title = taskMap.value("title").toString().toStdString();
    task.category = taskMap.value("category").toString().toStdString();
    task.description = taskMap.value("description").toString().toStdString();
    task.publish = taskMap.value("publish").toString().toStdString();
    task.due = taskMap.value("due").toString().toStdString();
    task.rating = taskMap.value("rating").toString().toStdString();
    task.finishStatus = taskMap.value("finishStatus", "未完成").toString().toStdString(); // 默认值
    task.comment = taskMap.value("comment").toString().toStdString();
    task.reviewString = taskMap.value("reviewString").toString().toStdString();
    task.resultRating = taskMap.value("resultRating").toString().toStdString();
    task.reviewStatus = taskMap.value("reviewStatus").toString().toStdString();

    if(auto taskManager = mCoreFramework->getTaskManager())
    {
        auto result = taskManager->addTask(task);
        if(result)
        {
            refreshTaskList();
        }
    }
}
