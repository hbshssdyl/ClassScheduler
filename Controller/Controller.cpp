#pragma once

#include "Controller.h"
#include "Utils/ControllerUtils.h"

Controller::Controller(QObject* parent)
{
    refreshOperateMode(OperateMode::WelcomePage);
    initialize();
}

void Controller::initialize()
{
    mAllOperateMode.clear();
    mAllOperateMode.emplace_back(OperateMode::Deduplication);
    mAllOperateMode.emplace_back(OperateMode::Enhancement);
    mAllOperateMode.emplace_back(OperateMode::Compression);
    onOperateModeSelected(OperateMode::WelcomePage);
}

std::string Controller::toOperateModeString(OperateMode mode)
{
    switch (mode)
    {
    case OperateMode::None:
        return "None";
    case OperateMode::WelcomePage:
        return "WelcomePage";
    case OperateMode::Deduplication:
        return "Deduplication";
    case OperateMode::Enhancement:
        return "Enhancement";
    case OperateMode::Compression:
        return "Compression";
    default:
        return "default";
    }

}

void Controller::refreshOperateMode(OperateMode mode)
{
    mOperateMode = mode;
    mLoadedView = QString::fromStdString(toOperateModeString(mode));
    emit operateModeChanged();
}

void Controller::onOperateModeSelected(OperateMode mode)
{
    cout << toOperateModeString(mode) << endl;
    refreshOperateMode(mode);

    QVariantList newActionItemsList;
    CUtils::updateActionItemsList(newActionItemsList, mOperateMode, mAllOperateMode);

    if (mActionItemsList != newActionItemsList)
    {
        mActionItemsList = std::move(newActionItemsList);
        emit actionItemsListChanged();
    }
}

void Controller::handleDeduplicationMode(string dirPath)
{
    std::string finalDirPath = dirPath.substr(8); // remove "file:///" in the dirPath
    auto files = CUtils::TraversingFilesRecursive(finalDirPath);

    //mRepeatedImagesGroup = OUtils::getRepeatedImages(files);

    QVariantList groupRepeatedImages;
    CUtils::updateRepeatedImages(groupRepeatedImages, mRepeatedImagesGroup);

    if (mGroupRepeatedImages != groupRepeatedImages)
    {
        mGroupRepeatedImages = std::move(groupRepeatedImages);
        emit repeatedImagesChanged();
    }
}

void Controller::onForderPathReceived(QString dirPath)
{
    if(mOperateMode == OperateMode::Deduplication)
        handleDeduplicationMode(dirPath.toStdString());
}

void Controller::onDeleteRepeatedImagesAction()
{
    for (auto& group : mRepeatedImagesGroup)
    {
        for (auto& image : group.images)
        {
            if (image.isReadyToDelete)
            {
                //OUtils::removeImage(image.path);
            }
        }
    }
}













































