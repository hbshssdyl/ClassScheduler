#ifndef TASKCONTROLLER_H
#define TASKCONTROLLER_H

#include <QObject>

class TaskController : public QObject
{
    Q_OBJECT
public:
    explicit TaskController(QObject *parent = nullptr);

signals:
};

#endif // TASKCONTROLLER_H
