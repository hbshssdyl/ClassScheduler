#ifndef FEEDBACKMANAGER_H
#define FEEDBACKMANAGER_H

#include "Utils/DataUtils.h"

using namespace ClassScheduler;

using CoreFrameworkPtr = std::shared_ptr<class CoreFramework>;

class StudentGradeManager
{
public:
    StudentGradeManager(CoreFrameworkPtr coreFramework);
    StudentGradeInfos getStudentGradeInfos();
    void initialize();
    void refreshDataFromServer();

    bool addStudentInfo(const StudentGradeInfo& studentInfo);
    bool addStudentGrade(const Grade& grade, int studentId);
    bool updateGrade(const Grade& grade);
    bool deleteGrade(int gradeId);

private:
    void initStudentGradeInfos();
    StudentGradeInfos getStudentGradeInfosFromServer();

private:
    std::weak_ptr<CoreFramework> mCoreFramework;
    StudentGradeInfos mStudentGradeInfos;
};

#endif // FEEDBACKMANAGER_H
