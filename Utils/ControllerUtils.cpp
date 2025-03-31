#pragma once

#include "ControllerUtils.h"
#include "Controller/Controller.h"

namespace fs = std::filesystem;

namespace PictureManager
{
QString toOperateModeString(Controller::OperateMode mode)
{
    switch (mode)
    {
    case Controller::OperateMode::None:
        return "None";
    case Controller::OperateMode::WelcomePage:
        return "WelcomePage";
    case Controller::OperateMode::Deduplication:
        return "Deduplication";
    case Controller::OperateMode::Enhancement:
        return "Enhancement";
    case Controller::OperateMode::Compression:
        return "Compression";
    default:
        return "default";
    }

}

void CUtils::updateActionItemsList(QVariantList& data, const Controller::OperateMode& selectedMode, const Controller::OperateModes& actionItems)
{
    for(auto activeItem : actionItems)
    {
        data.append(QVariantMap{ { "actionName", toOperateModeString(activeItem) },
                                { "OperateMode", static_cast<int>(activeItem) },
                                { "isSelected", activeItem == selectedMode }});
    }
}

void CUtils::updateRepeatedImages(QVariantList& data, const vector<RepeatedImages>& repeatedImagesGroup)
{
    for(auto repeatedImages : repeatedImagesGroup)
    {
        QVariantList images;
        for(auto& img : repeatedImages.images)
        {
            QVariantMap image;
            image.insert("name", QString::fromStdString(img.name));
            image.insert("path", QString::fromStdString("file:///" + img.path));
            image.insert("width", img.width);
            image.insert("height", img.height);
            image.insert("isReadyToDelete", img.isReadyToDelete);
            images.append(image);
        }
        data.append(QVariantMap{ { "images", images },
                                 { "maxWidth", repeatedImages.maxWidth },
                                 { "maxHeight", repeatedImages.maxHeight }});
    }
}

std::vector<std::string> CUtils::TraversingFilesRecursive(std::string dirName)
{
    std::vector<std::string> files = {};
    fs::path url(dirName);
    if (!fs::exists(url))
    {
        return files;
    }

    fs::recursive_directory_iterator begin(url);
    for (fs::recursive_directory_iterator end; begin != end; ++begin)
    {
        if (!fs::is_directory(begin->path()))
        {
            files.emplace_back(begin->path().generic_u8string());
            // 中文文件名乱码问题暂未解决
        }
    }
    return files;
}
} // namespace PictureManager
