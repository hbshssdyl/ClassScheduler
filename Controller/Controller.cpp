#include <iostream>
#include "Controller.h"
#include "Utils/ControllerUtils.h"
#include "Managers/UserManager.h"
#include "Managers/NetworkManager.h"

using namespace ClassScheduler;

Controller::Controller(QObject* parent)
    : QObject(parent)
{
    onOperateModeSelected(OperateMode::LoginView);
}

void Controller::init()
{
    QtConcurrent::run([this]() {
        initCoreFramework();
        refreshLatestVersionInfo();
    });
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

void Controller::refreshLatestVersionInfo()
{
    auto versionInfo = mCoreFramework->getVersionInformation();
    versionInfo.printInfo();
    QVariantMap newVersionInfoList;
    CUtils::updateVersionInfoList(newVersionInfoList, versionInfo);

    if (mVersionInfoList != newVersionInfoList)
    {
        mVersionInfoList = std::move(newVersionInfoList);
        emit versionInfoListChanged();
    }

    m_installProcess = new QProcess(this);  // 初始化进程
    connect(m_installProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Controller::onInstallFinished);
}

void Controller::updateAllData()
{
    QtConcurrent::run([this]() {
        if(mCoreFramework)
        {
            mCoreFramework->refreshDataFromServer();
            LOG_INFO("Success updateAllData");
        }
    });

    // 发射信号到主线程
    QMetaObject::invokeMethod(this, "dataUpdateFinished",
                              Qt::QueuedConnection);

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
    LOG_INFO("当前选中的mod: " +CUtils::toString(mode).toStdString());
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
    if(email.isEmpty() || username.isEmpty() || password.isEmpty()) {
        emit registerOrLoginResult("EmptyInfo");
        return;
    }

    if(auto networkManager = mCoreFramework->getNetworkManager())
    {
        auto result = networkManager->sendRegisterRequest(email.toStdString(), username.toStdString(), password.toStdString(), role.toStdString());
        LOG_INFO(result.statusStr);
        LOG_INFO(result.rawResponse);
        emit registerOrLoginResult(QString::fromStdString(result.statusStr));
    }

    // ResponseResult result;
    // result.username = "boss";
    // result.status = ResultStatus::LoginSuccess;
    // result.role = UserRole::Boss;

    // mCoreFramework->saveLoginUserInfo(result.username);
    // if(auto userManager = mCoreFramework->getUserManager())
    // {
    //     if(result.status == ResultStatus::LoginSuccess) {
    //         mUserInfo = userManager->getUserInfoByLoginInfo(result.username, result.role);
    //         if(!mUserInfo.name.empty())
    //         {
    //             mName = QString::fromStdString(mUserInfo.name);
    //             refreshActionItems();
    //             emit nameChanged();
    //         }
    //         onOperateModeSelected(OperateMode::WelcomePage);
    //     } else if(result.status == ResultStatus::UserOrEmailNotFound) {
    //         emit registerOrLoginResult("UserOrEmailNotFound");
    //     } else if(result.status == ResultStatus::PasswordIncorrect) {
    //         emit registerOrLoginResult("PasswordIncorrect");
    //     } else {
    //         emit registerOrLoginResult("LoginFailed");
    //     }
    // }
}

void Controller::onTryToLogin(QString login, QString password)
{
    // if(login.isEmpty() || password.isEmpty()) {
    //     emit registerOrLoginResult("EmptyInfo");
    //     return;
    // }


    if(auto networkManager = mCoreFramework->getNetworkManager())
    {
        // auto result = networkManager->sendLoginRequest(login.toStdString(), password.toStdString());

        // LOG_INFO(result.statusStr);
        // LOG_INFO(result.rawResponse);

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

void Controller::onUpdateStarted()
{
    LOG_INFO("onUpdateStarted");
    if (mCoreFramework) {
        mCoreFramework->updateAppToLatestVersion([this](int progress) {
            if (mDownloadValue != progress) {
                mDownloadValue = progress;
                emit downloadValueChanged();
            }
        });
    }
}

void Controller::installLatestVersion()
{
    auto exePath = QString::fromStdString(mCoreFramework->getVersionInformation().clientSavePath);
    if (exePath.isEmpty()) {
        LOG_INFO("EXE 路径为空，无法安装");
        return;
    }

    if (!QFile::exists(exePath)) {
        LOG_INFO("EXE 文件不存在：" + exePath.toStdString());
        return;
    }

    // 启动 EXE 进程（异步，非阻塞）
    QStringList arguments;
    arguments << "/S";  // 静默安装参数（NSIS 示例；Inno Setup 用 "/VERYSILENT /SUPPRESSMSGBOXES"）

    m_installProcess->start(exePath, arguments);
    m_installProcess->waitForStarted();  // 等待启动

    if (!m_installProcess->state()) {
        LOG_INFO("安装进程启动失败");
        return;
    }

    LOG_INFO("开始安装：" + exePath.toStdString());
}

void Controller::onInstallFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        LOG_INFO("安装成功完成");
        // 可选：显示成功消息（在 QML 中处理更好，避免阻塞）
        // QMessageBox::information(nullptr, "安装完成", "EXE 安装成功！");
    } else {
        LOG_INFO("安装失败，退出码：" + std::to_string(exitCode) + "，状态： QProcess::CrashExit");
        // 可选：显示错误消息
        // QMessageBox::warning(nullptr, "安装失败", "EXE 安装失败，请检查日志。");
    }
}
