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
        static void updateActionItemsList(QVariantList& data, const Controller::OperateMode& selectedMode, const Controller::OperateModes& actionItems);
        static void updateClassInfoList(QVariantList& data, ClassInfos& classInfos);
        static void updateTeacherHeaderList(QVariantList& data);
        static void doSearchClassInfos(ClassInfos& allInfos, ClassInfos& searchInfos, QString searchString);
    };

} // namespace PictureManager
