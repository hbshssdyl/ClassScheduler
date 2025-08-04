#ifndef SCHEDULECLASSCONTROLLER_H
#define SCHEDULECLASSCONTROLLER_H

#include <QObject>
#include "Utils/DataUtils.h"
#include "Managers/CoreFramework.h"

class ScheduleClassController : public QObject{
    Q_OBJECT

public:
    Q_PROPERTY(QList<QList<QVariantMap>> scheduleClassResultsList MEMBER mScheduleClassResultsList NOTIFY scheduleClassResultsListChanged)

public:
    explicit ScheduleClassController(CoreFrameworkPtr coreFramework, QObject* parent = nullptr);

public slots:
    void onRequiredInfosReceived(QVariantList requiredInfosList);

signals:
    void scheduleClassResultsListChanged();

private:
    void initialize();
    void updateScheduleClassResultsList(ScheduleClassInputInfo inputInfos);

private:
    std::weak_ptr<CoreFramework> mCoreFramework;
    ClassInfos mClassInfosFromDB;
    TeacherInfos mTeacherInfosFromDB;
    QVariantList mRequiredInfosList;
    QList<QList<QVariantMap>> mScheduleClassResultsList;
};

#endif // SCHEDULECLASSCONTROLLER_H
