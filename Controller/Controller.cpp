#pragma once

#include <iostream>
#include "Controller.h"
#include "Utils/ControllerUtils.h"

using namespace ClassScheduler;

Controller::Controller(QObject* parent)
{
    refreshOperateMode(OperateMode::WelcomePage);
    initialize();
}

void Controller::initialize()
{
    initDB();
    mAllOperateMode.clear();
    mAllOperateMode.emplace_back(OperateMode::SearchClassInfo);
    mAllOperateMode.emplace_back(OperateMode::SearchTeacherInfo);
    mAllOperateMode.emplace_back(OperateMode::ScheduleClass);
    mAllOperateMode.emplace_back(OperateMode::CalcOneToOneMoney);
    mAllOperateMode.emplace_back(OperateMode::CalcClassMoney);
    onOperateModeSelected(OperateMode::LoginView);
}

void Controller::initDB()
{
    mDataManager = std::make_shared<DataManager>();
    if(mDataManager)
    {
        mDataManager->createDBConnection();
        mDataManager->storeAllTableDataCount();
        mDataManager->refreshAllDataFromDB();
        mDataCount = QString::number(mDataManager->getTableDataCount(CLASS_INFOS_TABLE_NAME));
        emit dataCountChanged();
        cout << "initDB, currentDataCount: " << mDataCount.toStdString() << endl;
    }
}

QString Controller::toOperateModeString(OperateMode mode)
{
    switch (mode)
    {
    case OperateMode::None:
        return "None";
    case OperateMode::LoginView:
        return "LoginView";
    case OperateMode::FileView:
        return "FileView";
    case OperateMode::WelcomePage:
        return "WelcomePage";
    case OperateMode::SearchClassInfo:
        return "SearchClassInfo";
    case OperateMode::SearchTeacherInfo:
        return "SearchTeacherInfo";
    case OperateMode::ScheduleClass:
        return "ScheduleClass";
    case OperateMode::CalcOneToOneMoney:
        return "CalcOneToOneMoney";
    case OperateMode::CalcClassMoney:
        return "CalcClassMoney";
    default:
        return "default";
    }

}

void Controller::refreshOperateMode(OperateMode mode)
{
    mOperateMode = mode;
    mLoadedView = toOperateModeString(mode);
    switch (mode)
    {
        case OperateMode::None:
        case OperateMode::LoginView:
        case OperateMode::FileView:
        {
            mShowActions = false;
            break;
        }

        case OperateMode::SearchClassInfo:
        {
            if(mSearchTeacherInfoController)
            {
                mSearchTeacherInfoController->refreshSearchTeacherInfo();
            }
        }
        case OperateMode::SearchTeacherInfo:
        {
            if(mSearchClassInfoController)
            {
                mSearchClassInfoController->refreshSearchClassInfo();
            }
        }
        case OperateMode::WelcomePage:
        case OperateMode::ScheduleClass:
        case OperateMode::CalcOneToOneMoney:
        case OperateMode::CalcClassMoney:
        {
            mShowActions = true;
            break;
        }
        default:
            break;
    }
    emit operateModeChanged();
}

void Controller::onOperateModeSelected(OperateMode mode)
{
    cout << "当前选中的mod: " << toOperateModeString(mode).toStdString() << endl;
    refreshOperateMode(mode);

    QVariantList newActionItemsList;
    CUtils::updateActionItemsList(newActionItemsList, mOperateMode, mAllOperateMode);

    if (mActionItemsList != newActionItemsList)
    {
        mActionItemsList = std::move(newActionItemsList);
        emit actionItemsListChanged();
    }
}

void Controller::onTryToLogin(QString username, QString password)
{
    onOperateModeSelected(OperateMode::FileView);
}

void Controller::onFileUploaded(QString filePath)
{
    mNewDataFilePath = QUrl(filePath).toLocalFile();
    cout << "filePath: " << mNewDataFilePath.toStdString() << endl;

    auto fun = [this] {
        mDataManager->createDBConnection();
        if(mDataManager->refreshDBDataByFile(mNewDataFilePath))
        {
            onOperateModeSelected(OperateMode::WelcomePage);
        }
    };

    if(!mNewDataFilePath.isEmpty())
    {
        std::thread t1(fun);
        t1.detach();
    }
    else
    {
        onOperateModeSelected(OperateMode::WelcomePage);
    }
}

SearchClassInfoController* Controller::getSearchClassInfoController()
{
    if (!mSearchClassInfoController)
    {
        mSearchClassInfoController = new SearchClassInfoController(mDataManager, this);
    }
    return mSearchClassInfoController;
}

SearchTeacherInfoController* Controller::getSearchTeacherInfoController()
{
    if (!mSearchTeacherInfoController)
    {
        mSearchTeacherInfoController = new SearchTeacherInfoController(mDataManager, this);
    }
    return mSearchTeacherInfoController;
}

ScheduleClassController* Controller::getScheduleClassController()
{
    if (!mScheduleClassController)
    {
        mScheduleClassController = new ScheduleClassController(mDataManager, this);
    }
    return mScheduleClassController;
}













































