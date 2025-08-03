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
    initCoreFramework();
    initDB();
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

void Controller::initDB()
{
    getDatabaseFileAndRefreshAllData();
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
    case OperateMode::TaskAssistantView:
        return "TaskAssistantView";
    case OperateMode::TaskManagerView:
        return "TaskManagerView";
    case OperateMode::TeacherEvaluation:
        return "TeacherEvaluation";
    default:
        return "default";
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
    emit updateOperateMode(toOperateModeString(mode));
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

void Controller::onTryToRegister(QString email, QString username, QString password, QString role)
{
    if(email.isEmpty() || username.isEmpty() || password.isEmpty()) {
        emit registerOrLoginResult("EmptyInfo");
        return;
    }

    auto result = mNetworkManager->sendRegisterRequest(email.toStdString(), username.toStdString(), password.toStdString(), CUtils::toRoleString(role));
    cout << result.statusStr << endl;
    cout << result.rawResponse << endl;
    if(result.status == ResultStatus::RegisterSuccess) {
        emit registerOrLoginResult("RegisterSuccess");
    } else if(result.status == ResultStatus::UserExist) {
        emit registerOrLoginResult("UserExist");
    } else if(result.status == ResultStatus::EmailExist) {
        emit registerOrLoginResult("EmailExist");
    } else if(result.status == ResultStatus::EmailInvalid) {
        emit registerOrLoginResult("EmailInvalid");
    } else {
        emit registerOrLoginResult("RegisterFailed");
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
    result.username = "Dylan";
    result.status = ResultStatus::LoginSuccess;
    result.role = UserRole::SuperAdmin;

    if(result.status == ResultStatus::LoginSuccess) {
        mUserInfo = mUserManager->getUserInfoByLoginInfo(result.username, result.role);
        if(!mUserInfo.name.empty())
        {
            mName = QString::fromStdString(mUserInfo.name);
            refreshActionItems();
            emit nameChanged();
        }
        if(mAllDataIsReady)
        {
            onOperateModeSelected(OperateMode::WelcomePage);
        }
    } else if(result.status == ResultStatus::UserOrEmailNotFound) {
        emit registerOrLoginResult("UserOrEmailNotFound");
    } else if(result.status == ResultStatus::PasswordIncorrect) {
        emit registerOrLoginResult("PasswordIncorrect");
    } else {
        emit registerOrLoginResult("LoginFailed");
    }
}

void Controller::onFileUploaded(QString filePath)
{
    if (!mDataManager) {
        qWarning() << "DataManager 为空，无法处理文件：" << filePath;
        emit refreshDatabaseFinished();
        return;
    }

    mNewDataFilePath = QUrl(filePath).toLocalFile();
    cout << "filePath: " << mNewDataFilePath.toStdString() << endl;
    if(mNewDataFilePath.isEmpty())
    {
        qWarning() << "filePath 为空，无法处理文件：" << mNewDataFilePath;
        emit refreshDatabaseFinished();
        return;
    }

    // 使用 QtConcurrent 异步运行长时间任务
    QFuture<void> future = QtConcurrent::run([this]() {
        mDataManager->createDBConnection();
        if(mDataManager->refreshAllDataFromFile(mNewDataFilePath))
        {
            cout << "Refresh DB data by excel file" << endl;
        }
        mDataManager->closeDBConnection();
    });

    // 连接 QFutureWatcher 以处理任务完成
    disconnect(&mFutureWatcher, nullptr, this, nullptr);
    connect(&mFutureWatcher, &QFutureWatcher<void>::finished, this, [this]() {
        cout << "onFileUploaded 文件处理完成" << endl;
        refreshControllersData();
        auto result = mNetworkManager->uploadDbFile();
        cout << result.statusStr << endl;
        cout << result.rawResponse << endl;
        emit refreshDatabaseFinished();
    });

    // 设置 future 以监控任务完成
    mFutureWatcher.setFuture(future);
}

void Controller::getDatabaseFileAndRefreshAllData()
{
    if (!mDataManager || !mNetworkManager) {
        qWarning() << "DataManager 为空，无法处理数据文件";
        return;
    }

    // 使用 QtConcurrent 异步运行长时间任务
    QFuture<void> future = QtConcurrent::run([this]() {
        auto result = mNetworkManager->downloadDbFile();
        cout << result.statusStr << endl;
        cout << result.rawResponse << endl;
        if(result.status == ResultStatus::DatabaseFileDownloadSucess)
        {
            mDataManager->createDBConnection();

            mDataManager->refreshAllDataFromDB();
            mDataManager->storeAllTableDataCount();
            mDataCount = QString::number(mDataManager->getTableDataCount(CLASS_INFOS_TABLE_NAME));
            mDataManager->closeDBConnection();
            refreshControllersData();
            emit dataCountChanged();
        }
    });

    // 连接 QFutureWatcher 以处理任务完成
    disconnect(&mFutureWatcher, nullptr, this, nullptr);
    connect(&mFutureWatcher, &QFutureWatcher<void>::finished, this, [this]() {
        cout << "getDatabaseFileAndRefreshAllData 文件处理完成" << endl;
        mAllDataIsReady = true;
    });

    // 设置 future 以监控任务完成
    mFutureWatcher.setFuture(future);
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













































