#ifndef TASKCONTROLLER_H
#define TASKCONTROLLER_H

#include <QObject>
#include "Managers/CoreFramework.h"

class TaskController : public QObject
{
    Q_OBJECT
public:
    explicit TaskController(CoreFrameworkPtr& coreFramework, QObject *parent = nullptr);

private:
    CoreFrameworkPtr mCoreFramework;
};

#endif // TASKCONTROLLER_H
