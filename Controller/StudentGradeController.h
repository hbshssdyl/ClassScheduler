#pragma once

#include "Utils/DataUtils.h"
#include "Managers/CoreFramework.h"

#include <QObject>
#include <cstdio>
#include <QString>
#include <QVariant>

using namespace std;
using namespace ClassScheduler;

class StudentGradeController : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(QVariantMap studentGradeList MEMBER mStudentGradeList NOTIFY studentGradeListChanged)

public:
    explicit StudentGradeController(CoreFrameworkPtr coreFramework, QObject* parent = nullptr);
    void refreshStudentGradeList();

signals:
    void studentGradeListChanged();

public slots:
    void onSearchTriggered(QString searchString);

private:
    void initialize();
    void updateStudentGradeListChanged(StudentInfos& studentInfos/*, TeacherStudentInfos& teacherStudentInfos, TeacherStudentBasicInfo& studentBasicInfo*/);

private:
    QVariantList mStudentGradeList;
    StudentInfos mStudentInfosFromDB;
    AppSettings mAppSettings;
    std::weak_ptr<CoreFramework> mCoreFramework;
};

