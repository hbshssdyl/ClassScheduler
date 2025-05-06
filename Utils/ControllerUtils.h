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
        static void updateClassHeaderList(QVariantList& data);
        static void doSearchClassInfos(ClassInfos& allInfos, ClassInfos& searchInfos, QString searchString);

        //For SearchTeacherInfoController.cpp
        static void updateTeacherInfoList(QVariantList& data, TeacherInfos& teacherInfos);
        static void updateTeacherHeaderList(QVariantList& data);
        static void doSearchTeacherInfos(TeacherInfos& allInfos, TeacherInfos& searchInfos, QString searchString);
    };

} // namespace PictureManager
