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

private:
    void initialize();
    void initTasks();
    void initTaskFromServer();
    std::string getCurrentDate();
    Task createTask(const TaskTemplate& tmpl, const std::string& date);
    Tasks generateDailyTasks();

private:
    std::weak_ptr<CoreFramework> mCoreFramework;
    Tasks mTasks;
};

#endif // TASKMANAGER_H
