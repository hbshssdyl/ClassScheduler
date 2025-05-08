#ifndef SCHEDULECLASSCONTROLLER_H
#define SCHEDULECLASSCONTROLLER_H

#include <QObject>
#include "Managers/DataManager.h"

class ScheduleClassController : public QObject{
    Q_OBJECT
public:
    explicit ScheduleClassController(DataManagerPtr DataManager, QObject* parent = nullptr);

public slots:
    void onRequiredInfosReceived(QVariantList requiredInfosList);

// signals:
private:
    void initialize();

private:
    DataManagerPtr mDataManager;
    ClassInfos mClassInfosFromDB;
    TeacherInfos mTeacherInfosFromDB;
};

#endif // SCHEDULECLASSCONTROLLER_H
