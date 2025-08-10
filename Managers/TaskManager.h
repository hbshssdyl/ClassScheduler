#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Utils/DataUtils.h"

using namespace ClassScheduler;

using CoreFrameworkPtr = std::shared_ptr<class CoreFramework>;

class TaskManager : public std::enable_shared_from_this<TaskManager>
{
public:
    TaskManager(CoreFrameworkPtr coreFramework);
    Tasks getTasks();
    void initialize();

    bool addTask(Task task);
    bool updateTaskFinishStatus(int taskId, std::string commentString);
    bool updateTaskReviewStatus(int taskId, std::string resultRating, std::string reviewString);

private:
    void initTasks();
    void initDailyTasks();
    void initWeeklyTasks();
    void initMonthlyTasks();
    void refreshTaskStatus();
    Tasks getTaskFromServer();
    Task createTask(const TaskTemplate& tmpl, const std::string& publishDate, const std::string& dueDate);
    Tasks generateDailyTasks();
    Tasks generateWeeklyTasks();
    Tasks generateMonthlyTasks();

private:
    std::weak_ptr<CoreFramework> mCoreFramework;
    Tasks mTasks;
};

#endif // TASKMANAGER_H
