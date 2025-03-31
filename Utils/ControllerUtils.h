#pragma once

#include "DataUtils.h"
#include "Controller/Controller.h"
#include <QString>
#include <QVariant>
#include <QVariantMap>
#include <filesystem>
#include <vector>
#include <io.h>

using namespace std;

namespace PictureManager
{
class CUtils
{
public:
    static void updateActionItemsList(QVariantList& data, const Controller::OperateMode& selectedMode, const Controller::OperateModes& actionItems);
    static void updateRepeatedImages(QVariantList& data, const vector<RepeatedImages>& repeatedImagesGroup);
    static vector<string> TraversingFilesRecursive(string dirName);
};

} // namespace PictureManager
