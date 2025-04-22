#pragma once

#include <iostream>
#include "Controller.h"
#include "Utils/ControllerUtils.h"
#include <QtSql>

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
    getTeacherInfosByExcelFile("FinalTest.xlsx");
}

void createDBConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("teacherInfos.db");
    if (!db.open()) {
        qDebug() << "Failed to connect database.";
        return;
    }
    // std::string teacherName;

    // //teacherCourseList
    // int date;
    // std::string studentName;
    // std::string weekend;
    // std::string school;
    // int studentPhoneNubmer;
    // std::string grade;
    // std::string suject;
    // std::string time;
    // std::string type;
    // int courseTime;
    // int studentFee;
    // int teacherFee;
    // 创建 teacherInfos 表
    QSqlQuery query;
    bool ret = query.exec("CREATE TABLE IF NOT EXISTS teacherInfos("
                          "id                 INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "teacherName        CHAR(50)            NOT NULL,"
                          "teacherNickName    CHAR(50)            NOT NULL,"
                          "date               INTEGER,"
                          "studentName        CHAR(50),"
                          "weekend            CHAR(50),"
                          "school             CHAR(50),"
                          "studentPhoneNubmer INTEGER,"
                          "grade              CHAR(50),"
                          "suject             CHAR(50)            NOT NULL,"
                          "time               CHAR(50)            NOT NULL,"
                          "type               CHAR(50),"
                          "courseTime         INTEGER             NOT NULL,"
                          "studentFee         INTEGER,"
                          "teacherFee         INTEGER"
                          ")");
    if (!ret) {
        qDebug() << "Failed to create table: " << query.lastError().text();
        return;
    }
}

void insertData(QString teacherName, QString suject, QString time, int courseTime)
{
    QSqlQuery query;
    QString sql = QString("INSERT INTO teacherInfos (teacherName, suject, time, courseTime) "
                          "VALUES (%1, '数学', '13:00-17:00', 2)").arg(teacherName);
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
        cout << "Id:" << id << " teacherName:" << teacherName.toStdString() << " suject:" << suject.toStdString() << " time:" << time.toStdString() << " courseTime:" << courseTime << endl;
    }
}

void Controller::getTeacherInfosByExcelFile(QString filePath)
{
    vector<teacherInfo> teacherInfos;
    CUtils::getTeacherInfosFromExcelFile(teacherInfos, filePath);
    createDBConnection();
    for(auto teacherInfo : teacherInfos)
    {
        insertData(teacherInfo.teacherName,
                   teacherInfo.teacherName,
teacherInfo.teacherName,
teacherInfo.teacherName,
teacherInfo.teacherName,
teacherInfo.teacherName,
teacherInfo.teacherName,
teacherInfo.teacherName,
teacherInfo.teacherName,
    }
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

SearchTeacherInfoController* Controller::getSearchTeacherInfoController()
{
    if (!mSearchTeacherInfoController)
    {
        mSearchTeacherInfoController = new SearchTeacherInfoController(this);
        mSearchTeacherInfoController->initialize();
    }
    return mSearchTeacherInfoController;
}













































