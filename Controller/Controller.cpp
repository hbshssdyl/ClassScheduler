#pragma once

#include <iostream>
#include "Controller.h"
#include "Utils/ControllerUtils.h"
#include <QtSql>
#include <thread>

using namespace ClassScheduler;

Controller::Controller(QObject* parent)
{
    refreshOperateMode(OperateMode::WelcomePage);
    initialize();
}

void Controller::initialize()
{
    mAllOperateMode.clear();
    mAllOperateMode.emplace_back(OperateMode::SearchTeacherInfo);
    mAllOperateMode.emplace_back(OperateMode::ScheduleClass);
    mAllOperateMode.emplace_back(OperateMode::CalcOneToOneMoney);
    mAllOperateMode.emplace_back(OperateMode::CalcClassMoney);
    onOperateModeSelected(OperateMode::WelcomePage);
    getTeacherInfosByExcelFile("test.xlsx");
}

void createDBConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("teacherInfos.db");
    if (!db.open()) {
        qDebug() << "Failed to connect database.";
        return;
    }
    QSqlQuery query;
    bool ret = query.exec("CREATE TABLE IF NOT EXISTS teacherInfos("
                          "id                 INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "date               CHAR(50),"
                          "weekend            CHAR(50)            NOT NULL,"
                          "studentName        CHAR(50)            NOT NULL,"
                          "school             CHAR(50),"
                          "studentPhoneNubmer CHAR(50),"
                          "grade              CHAR(50),"
                          "suject             CHAR(50),"
                          "time               CHAR(50)            NOT NULL,"
                          "teacherNickName    CHAR(50)            NOT NULL,"
                          "learningType       CHAR(50)            NOT NULL,"
                          "courseTime         CHAR(50),"
                          "studentFee         CHAR(50)            NOT NULL,"
                          "studentTotalFee    CHAR(50),"
                          "teacherName        CHAR(50),"
                          "teacherFee         CHAR(50),"
                          "gotMoney           CHAR(50),"
                          "payType            CHAR(50),"
                          "payDate            CHAR(50)"
                          ")");
    if (!ret) {
        qDebug() << "Failed to create table: " << query.lastError().text();
        return;
    }
}

void insertData(teacherInfo info)
{
    QSqlQuery query;
    QString sql = QString("INSERT INTO teacherInfos (date, weekend, studentName, school, studentPhoneNubmer, grade,"
                                                    "suject, time, teacherNickName, learningType, courseTime, studentFee,"
                                                    "studentTotalFee, teacherName, teacherFee, gotMoney, payType, payDate)"
                          "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13', '%14', '%15', '%16', '%17', '%18')")
                          .arg(info.date).arg(info.weekend).arg(info.studentName).arg(info.school).arg(info.studentPhoneNubmer).arg(info.grade)
                          .arg(info.suject).arg(info.time).arg(info.teacherNickName).arg(info.learningType).arg(info.courseTime).arg(info.studentFee)
                          .arg(info.studentTotalFee).arg(info.teacherName).arg(info.teacherFee).arg(info.gotMoney).arg(info.payType).arg(info.payDate);
    bool ret = query.exec(sql);
    if (!ret) {
        qDebug() << "Failed to insert data: " << query.lastError().text();
    }
}

void queryData()
{
    QSqlQuery query("SELECT * FROM teacherInfos");
    while (query.next()) {
        int id = query.value("id").toInt();
        QString teacherName = query.value("teacherName").toString();
        QString suject = query.value("suject").toString();
        QString time = query.value("time").toString();
        int courseTime = query.value("courseTime").toInt();
    }
}

void Controller::getTeacherInfosByExcelFile(QString filePath)
{
    auto f = [this, filePath] {
        CUtils::getTeacherInfosFromExcelFile(mTeacherInfos, filePath);
        createDBConnection();
        for(auto info : mTeacherInfos)
        {
            insertData(info);
        }
        mTeacherInfosCondition.notify_all();
    };

    std::thread t1(f);
    t1.detach();
}

std::string Controller::toOperateModeString(OperateMode mode)
{
    switch (mode)
    {
    case OperateMode::None:
        return "None";
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

void Controller::waitTeacherInfosInited()
{
    std::mutex teacherInfosMutex;
    std::unique_lock<std::mutex> u_lk(teacherInfosMutex);
    int timeout = 5;

    if (mTeacherInfos.size() > 0)
    {
        return;
    }

    std::cv_status ret = mTeacherInfosCondition.wait_for(u_lk, std::chrono::seconds(timeout));
    if (ret == std::cv_status::timeout)
    {
        cout << "Maybe there is something wrong with the mTeacherInfos" << endl;
    }
    else if (ret == std::cv_status::no_timeout)
    {
        cout << "mTeacherInfos Initialized" << endl;
    }
}

SearchTeacherInfoController* Controller::getSearchTeacherInfoController()
{
    if (!mSearchTeacherInfoController)
    {
        mSearchTeacherInfoController = new SearchTeacherInfoController(this);
        waitTeacherInfosInited();
        mSearchTeacherInfoController->initialize(mTeacherInfos);
    }
    return mSearchTeacherInfoController;
}













































