#include <iostream>
#include "Controller.h"
#include "Utils/ControllerUtils.h"
#include "Managers/NetworkManager.h"
#include "Managers/UserManager.h"

using namespace ClassScheduler;

Controller::Controller(QObject* parent)
{
    initialize();
}

void Controller::initialize()
{
    initCoreFramework();
    onOperateModeSelected(OperateMode::LoginView);
}

void Controller::initCoreFramework()
{
    mCoreFramework = std::make_shared<CoreFramework>();
    if(mCoreFramework)
    {
        mCoreFramework->initialize();
    }
}

void Controller::refreshActionItems()
{
    mAllOperateMode.clear();
    for(auto& mod : mUserInfo.mods)
    {
        mAllOperateMode.emplace_back(mod);
    }
}

void Controller::refreshOperateMode(OperateMode mode)
{
    mOperateMode = mode;
    switch (mode)
    {
        case OperateMode::None:
        case OperateMode::LoginView:
        {
            mShowActions = false;
            mShowUserInfo = false;
            break;
        }
        case OperateMode::SearchClassInfo:
        case OperateMode::SearchTeacherInfo:
        case OperateMode::SearchStudentInfo:
        case OperateMode::WelcomePage:
        case OperateMode::ScheduleClass:
        case OperateMode::FileView:
        case OperateMode::TaskAssistantView:
        case OperateMode::TaskManagerView:
        case OperateMode::FeedbackApp:
        case OperateMode::FeedbackCompany:
        case OperateMode::AccountView:
        case OperateMode::TeacherEvaluation:
        {
            mShowActions = true;
            mShowUserInfo = true;
            break;
        }
        default:
            break;
    }
    emit operateModeChanged();
    emit updateOperateMode(CUtils::toString(mode));
}

void Controller::onOperateModeSelected(OperateMode mode)
{
    cout << "当前选中的mod: " << CUtils::toString(mode).toStdString() << endl;
    refreshOperateMode(mode);

    QVariantList newActionItemsList;
    CUtils::updateActionItemsList(newActionItemsList, mOperateMode, mAllOperateMode);

    if (mActionItemsList != newActionItemsList)
    {
        mActionItemsList = std::move(newActionItemsList);
        emit actionItemsListChanged();
    }
}

void Controller::onTryToRegister(QString email, QString username, QString password, QString role)
{
    // if(email.isEmpty() || username.isEmpty() || password.isEmpty()) {
    //     emit registerOrLoginResult("EmptyInfo");
    //     return;
    // }

    // if(auto networkManager = mCoreFramework->getNetworkManager())
    // {
    //     auto result = networkManager->sendRegisterRequest(email.toStdString(), username.toStdString(), password.toStdString(), role.toStdString());
    //     cout << result.statusStr << endl;
    //     cout << result.rawResponse << endl;
    //     emit registerOrLoginResult(QString::fromStdString(result.statusStr));
    // }
    ResponseResult result;
    result.username = "boss";
    result.status = ResultStatus::LoginSuccess;
    result.role = UserRole::Boss;

    mCoreFramework->saveLoginUserInfo(result.username);
    if(auto userManager = mCoreFramework->getUserManager())
    {
        if(result.status == ResultStatus::LoginSuccess) {
            mUserInfo = userManager->getUserInfoByLoginInfo(result.username, result.role);
            if(!mUserInfo.name.empty())
            {
                mName = QString::fromStdString(mUserInfo.name);
                refreshActionItems();
                emit nameChanged();
            }
            onOperateModeSelected(OperateMode::WelcomePage);
        } else if(result.status == ResultStatus::UserOrEmailNotFound) {
            emit registerOrLoginResult("UserOrEmailNotFound");
        } else if(result.status == ResultStatus::PasswordIncorrect) {
            emit registerOrLoginResult("PasswordIncorrect");
        } else {
            emit registerOrLoginResult("LoginFailed");
        }
    }
}

void Controller::onTryToLogin(QString login, QString password)
{
    // if(login.isEmpty() || password.isEmpty()) {
    //     emit registerOrLoginResult("EmptyInfo");
    //     return;
    // }

    // auto result = mNetworkManager->sendLoginRequest(login.toStdString(), password.toStdString());

    // cout << result.statusStr << endl;
    // cout << result.rawResponse << endl;

    ResponseResult result;
    result.username = "Dylandu";
    result.status = ResultStatus::LoginSuccess;
    result.role = UserRole::SuperAdmin;

    mCoreFramework->saveLoginUserInfo(result.username);
    if(auto userManager = mCoreFramework->getUserManager())
    {
        if(result.status == ResultStatus::LoginSuccess) {
            mUserInfo = userManager->getUserInfoByLoginInfo(result.username, result.role);
            if(!mUserInfo.name.empty())
            {
                mName = QString::fromStdString(mUserInfo.name);
                refreshActionItems();
                emit nameChanged();
            }
            onOperateModeSelected(OperateMode::WelcomePage);
        } else if(result.status == ResultStatus::UserOrEmailNotFound) {
            emit registerOrLoginResult("UserOrEmailNotFound");
        } else if(result.status == ResultStatus::PasswordIncorrect) {
            emit registerOrLoginResult("PasswordIncorrect");
        } else {
            emit registerOrLoginResult("LoginFailed");
        }
    }
}

void Controller::refreshControllersData()
{
    if(mSearchClassInfoController)
    {
        mSearchClassInfoController->refreshSearchClassInfo();
    }
    if(mSearchTeacherInfoController)
    {
        mSearchTeacherInfoController->refreshSearchTeacherInfo();
    }
    if(mSearchStudentInfoController)
    {
        mSearchStudentInfoController->refreshSearchStudentInfo();
    }
}

SearchClassInfoController* Controller::getSearchClassInfoController()
{
    if (!mSearchClassInfoController)
    {
        mSearchClassInfoController = new SearchClassInfoController(mCoreFramework, this);
    }
    return mSearchClassInfoController;
}

SearchTeacherInfoController* Controller::getSearchTeacherInfoController()
{
    if (!mSearchTeacherInfoController)
    {
        mSearchTeacherInfoController = new SearchTeacherInfoController(mCoreFramework, this);
    }
    return mSearchTeacherInfoController;
}

SearchStudentInfoController* Controller::getSearchStudentInfoController()
{
    if (!mSearchStudentInfoController)
    {
        mSearchStudentInfoController = new SearchStudentInfoController(mCoreFramework, this);
    }
    return mSearchStudentInfoController;
}

ScheduleClassController* Controller::getScheduleClassController()
{
    if (!mScheduleClassController)
    {
        mScheduleClassController = new ScheduleClassController(mCoreFramework, this);
    }
    return mScheduleClassController;
}

DatabaseController* Controller::getDatabaseController()
{
    if (!mDatabaseController)
    {
        mDatabaseController = new DatabaseController(mCoreFramework, this);
        connect(mDatabaseController, &DatabaseController::refreshDatabaseFinished, this, &Controller::refreshControllersData);
    }
    return mDatabaseController;
}

TaskController* Controller::getTaskController()
{
    if (!mTaskController)
    {
        mTaskController = new TaskController(mCoreFramework, this);
    }
    return mTaskController;
}

AccountViewController* Controller::getAccountViewController()
{
    if (!mAccountViewController)
    {
        mAccountViewController = new AccountViewController(mCoreFramework, this);
    }
    return mAccountViewController;
}

FeedbackController* Controller::getFeedbackController()
{
    if (!mFeedbackController)
    {
        mFeedbackController = new FeedbackController(mCoreFramework, this);
    }
    return mFeedbackController;
}












































