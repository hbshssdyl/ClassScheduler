#pragma once

#include "Controller/Controller.h"
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
    static void updateTeacherInfoList(QVariantList& data);
};

} // namespace PictureManager
