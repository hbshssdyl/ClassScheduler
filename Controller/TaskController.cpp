#include "TaskController.h"

TaskController::TaskController(CoreFrameworkPtr& coreFramework, QObject *parent)
    : mCoreFramework(coreFramework)
    , QObject{parent}
{}
