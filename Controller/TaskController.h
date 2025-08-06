#ifndef TASKCONTROLLER_H
#define TASKCONTROLLER_H

#include <QObject>
#include "Managers/CoreFramework.h"

class TaskController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList taskList MEMBER mTaskList NOTIFY taskListChanged)

public:
    explicit TaskController(CoreFrameworkPtr& coreFramework, QObject *parent = nullptr);

private:
    void initialize();
    void refreshTaskList();

signals:
    void taskListChanged();

private:
    CoreFrameworkPtr mCoreFramework;
    QVariantList mTaskList;
    Tasks mTasks;
};

#endif // TASKCONTROLLER_H
