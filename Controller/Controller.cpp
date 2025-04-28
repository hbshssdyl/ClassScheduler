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
    mAllOperateMode.emplace_back(OperateMode::SearchTeacherInfo);
    mAllOperateMode.emplace_back(OperateMode::ScheduleClass);
    mAllOperateMode.emplace_back(OperateMode::CalcOneToOneMoney);
    mAllOperateMode.emplace_back(OperateMode::CalcClassMoney);
    onOperateModeSelected(OperateMode::LoginView);
}

void Controller::initDB()
{
    mDBManager = std::make_shared<DBManager>();
    if(mDBManager)
    {
        mDBManager->createDBConnection();
        mDBManager->storeAllTableDataCount();
        mDataCount = QString::number(mDBManager->getTableDataCount(TEACHER_INFOS_TABLE_NAME));
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
        case OperateMode::WelcomePage:
        case OperateMode::SearchTeacherInfo:
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
        if(mDBManager->refreshDBDataByFile(mNewDataFilePath, true))
        {
            mDBManager->storeAllTableDataCount();
            onOperateModeSelected(OperateMode::WelcomePage);
        }
    };

    if(!mNewDataFilePath.isEmpty())
    {
        std::thread t1(fun);
        t1.detach();
    }
}

SearchTeacherInfoController* Controller::getSearchTeacherInfoController()
{
    if (!mSearchTeacherInfoController)
    {
        mSearchTeacherInfoController = new SearchTeacherInfoController(mDBManager, this);
        mSearchTeacherInfoController->initialize();
    }
    return mSearchTeacherInfoController;
}













































