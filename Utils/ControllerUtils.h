#pragma once

#include "Controller/Controller.h"
#include "DataUtils.h"
#include <QString>
#include <QVariant>
#include <QVariantMap>
#include <vector>
#include <io.h>

using namespace std;

namespace ClassScheduler
{
    class CUtils
    {
    public:
        //For Controller.cpp
        static void updateActionItemsList(QVariantList& data, const Controller::OperateMode& selectedMode, const Controller::OperateModes& actionItems);

        //For SearchClassInfoController.cpp
        static void updateClassInfoList(QVariantMap& data, ClassInfos& classInfos);
        static void doSearchClassInfos(ClassInfos& allInfos, ClassInfos& searchInfos, QString searchString);

        //For SearchTeacherInfoController.cpp
        static void updateTeacherInfoList(QVariantMap& data, TeacherInfos& teacherInfos);
        static void doSearchTeacherInfos(TeacherInfos& allInfos, TeacherInfos& searchInfos, QString searchString);

        //For SearchStudentInfoController.cpp
        static void updateStudentInfoList(QVariantMap& data, StudentInfos& teacherInfos/*, TeacherStudentInfos& teacherStudentInfos, TeacherStudentBasicInfo& studentBasicInfo*/);
        static void doSearchStudentInfos(StudentInfos& allInfos, StudentInfos& searchInfos, QString searchString);

        //For ScheduleClassController.cpp
        static bool isTimeOverlap(const QString& timeRange1, const QString& timeRange2);
        static void updateScheduleClassResultsList(QList<QList<QVariantMap>>& data, ScheduleClassInputInfo& inputInfos, ClassInfos& classInfosList);
    };

} // namespace PictureManager
