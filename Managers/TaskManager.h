#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Utils/DataUtils.h"

using namespace ClassScheduler;

using CoreFrameworkPtr = std::shared_ptr<class CoreFramework>;

class TaskManager : public std::enable_shared_from_this<TaskManager>
{
public:
    TaskManager(CoreFrameworkPtr coreFramework);

private:
    std::weak_ptr<CoreFramework> mCoreFramework;
};

#endif // TASKMANAGER_H
