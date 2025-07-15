#pragma once

#include <iostream>
#include "Controller.h"
#include "Utils/ControllerUtils.h"

using namespace ClassScheduler;

Controller::Controller(QObject* parent)
{
    initialize();
}

void Controller::initialize()
{
    initManagers();
    initDB();
    onOperateModeSelected(OperateMode::LoginView);
}

void Controller::initManagers()
{
    mDataManager = std::make_shared<DataManager>();
    mUserManager = std::make_shared<UserManager>();
    mNetworkManager = std::make_shared<NetworkManager>();
}

void Controller::initDB()
{
    mDataManager = std::make_shared<DataManager>();
    if(mDataManager)
    {
        mDataManager->createDBConnection();
        mDataManager->storeAllTableDataCount();
        mDataCount = QString::number(mDataManager->getTableDataCount(CLASS_INFOS_TABLE_NAME));
        refreshAppSettings();
        mDataManager->closeDBConnection();
        emit dataCountChanged();
        cout << "initDB, currentDataCount: " << mDataCount.toStdString() << endl;
    }
}

void Controller::refreshAppSettings()
{
    auto appSettings = mDataManager->getAppSettingsFromDB();
    for(auto& setting : appSettings)
    {
        mAppSettings[setting.key] = setting.value;
    }
    emit appSettingsChanged();
}

void Controller::refreshActionItems()
{
    mAllOperateMode.clear();
    for(auto& mod : mUserInfo.mods)
    {
        mAllOperateMode.emplace_back(mod);
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
    case OperateMode::SearchStudentInfo:
        return "SearchStudentInfo";
    case OperateMode::ScheduleClass:
        return "ScheduleClass";
    case OperateMode::TeacherEvaluation:
        return "TeacherEvaluation";
    default:
        return "default";
    }

}

void Controller::refreshOperateMode(OperateMode mode)
{
    mOperateMode = mode;
    mLoadedView = toOperateModeString(mode);
    mShowUserInfo = true;
    switch (mode)
    {
        case OperateMode::None:
        case OperateMode::LoginView:
        case OperateMode::FileView:
        {
            mShowActions = false;
            mShowUserInfo = false;
            break;
        }

        case OperateMode::SearchClassInfo:
        {
            if(mSearchTeacherInfoController)
            {
                mSearchTeacherInfoController->refreshSearchTeacherInfo();
            }
            mShowActions = true;
            break;
        }
        case OperateMode::SearchTeacherInfo:
        {
            if(mSearchClassInfoController)
            {
                mSearchClassInfoController->refreshSearchClassInfo();
            }
            mShowActions = true;
            break;
        }
        case OperateMode::WelcomePage:
        case OperateMode::ScheduleClass:
        case OperateMode::SearchStudentInfo:
        case OperateMode::TeacherEvaluation:
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
    if(username.isEmpty() || password.isEmpty()) {
        qWarning() << "用户名或密码为空";
        return;
    }

    mNetworkManager->sendLoginRequest(username.toStdString(), password.toStdString());
    return;



    qDebug() << username << password;

    auto loginInfo = mDataManager->getLoginInfoFromDB();
    std::string stdUsername = username.toStdString();
    std::string stdPassword = password.toStdString();
    mUserInfo = mUserManager->getUserInfoByLoginInfo(stdUsername, stdPassword, loginInfo);
    if(!mUserInfo.name.empty())
    {
        mName = QString::fromStdString(mUserInfo.name);
        refreshActionItems();
        emit nameChanged();
        onOperateModeSelected(OperateMode::FileView);
    }
}

void Controller::onFileUploaded(QString filePath)
{
    if (!mDataManager) {
        qWarning() << "DataManager 为空，无法处理文件：" << filePath;
        return;
    }

    mNewDataFilePath = QUrl(filePath).toLocalFile();
    cout << "filePath: " << mNewDataFilePath.toStdString() << endl;

    // 使用 QtConcurrent 异步运行长时间任务
    QFuture<void> future = QtConcurrent::run([this]() {
        mDataManager->createDBConnection();
        if(!mNewDataFilePath.isEmpty())
        {
            if(mDataManager->refreshAllDataFromFile(mNewDataFilePath))
            {
                cout << "Refresh DB data by excel file" << endl;
            }
        }
        else{
            mDataManager->refreshAllDataFromDB();
        }
        mDataManager->closeDBConnection();
        onOperateModeSelected(OperateMode::WelcomePage);
    });

    // 连接 QFutureWatcher 以处理任务完成
    connect(&mFutureWatcher, &QFutureWatcher<void>::finished, this, []() {
        cout << "文件处理完成" << endl;
    });

    // 设置 future 以监控任务完成
    mFutureWatcher.setFuture(future);
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

SearchStudentInfoController* Controller::getSearchStudentInfoController()
{
    if (!mSearchStudentInfoController)
    {
        mSearchStudentInfoController = new SearchStudentInfoController(mDataManager, this);
    }
    return mSearchStudentInfoController;
}

ScheduleClassController* Controller::getScheduleClassController()
{
    if (!mScheduleClassController)
    {
        mScheduleClassController = new ScheduleClassController(mDataManager, this);
    }
    return mScheduleClassController;
}













































