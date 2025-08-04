#pragma once

#include "Utils/DataUtils.h"
#include "Managers/CoreFramework.h"

#include <QObject>
#include <cstdio>
#include <QString>
#include <QVariant>

using namespace std;
using namespace ClassScheduler;

class SearchStudentInfoController : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(QVariantMap studentInfoMap MEMBER mStudentInfoMap NOTIFY studentInfoMapChanged)

public:
    explicit SearchStudentInfoController(CoreFrameworkPtr coreFramework, QObject* parent = nullptr);
    void refreshSearchStudentInfo();

signals:
    void studentInfoMapChanged();

public slots:
    void onSearchTriggered(QString searchString);

private:
    void initialize();
    void updateStudentInfosList(StudentInfos& studentInfos/*, TeacherStudentInfos& teacherStudentInfos, TeacherStudentBasicInfo& studentBasicInfo*/);

private:
    QVariantMap mStudentInfoMap;
    StudentInfos mStudentInfosFromDB;
    AppSettings mAppSettings;
    std::weak_ptr<CoreFramework> mCoreFramework;
};

