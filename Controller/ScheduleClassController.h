#ifndef SCHEDULECLASSCONTROLLER_H
#define SCHEDULECLASSCONTROLLER_H

#include <QObject>
#include "Utils/DataUtils.h"
#include "Managers/DataManager.h"

class ScheduleClassController : public QObject{
    Q_OBJECT

public:
    Q_PROPERTY(QList<QList<QVariantMap>> scheduleClassResultsList MEMBER mScheduleClassResultsList NOTIFY scheduleClassResultsListChanged)

public:
    explicit ScheduleClassController(DataManagerPtr DataManager, QObject* parent = nullptr);

public slots:
    void onRequiredInfosReceived(QVariantList requiredInfosList);

signals:
    void scheduleClassResultsListChanged();

private:
    void initialize();
    void updateScheduleClassResultsList(ScheduleClassInputInfo inputInfos);

private:
    DataManagerPtr mDataManager;
    ClassInfos mClassInfosFromDB;
    TeacherInfos mTeacherInfosFromDB;
    QVariantList mRequiredInfosList;
    QList<QList<QVariantMap>> mScheduleClassResultsList;
};

#endif // SCHEDULECLASSCONTROLLER_H
