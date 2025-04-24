#include "DBManager.h"
#include <QtSql>
#include <iostream>

using namespace ClassScheduler;

DBManager::DBManager() {}

bool DBManager::createDBConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("ClassScheduler.db");
    if (!db.open()) {
        cout << "Failed to connect database." << endl;
        return false;
    }
    return true;
}

bool DBManager::createTeacherInfosTable()
{
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
        cout << "Failed to create table: " << query.lastError().text().toStdString() << endl;
        return false;
    }
    return true;
}

bool DBManager::insertDataToTeacherInfosTable(TeacherInfos& infos)
{
    for(auto info : infos)
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
            cout << "Failed to insert data: " << query.lastError().text().toStdString() << endl;
            return false;
        }
    }
    return true;
}

void DBManager::queryDataFromTeacherInfosTable(TeacherInfos& infos)
{
    QSqlQuery query("SELECT * FROM teacherInfos");
    cout << "mytest1" << endl;
    int cnt = 1;
    while (query.next()) {
        TeacherInfo info;
        info.teacherName = query.value("teacherName").toString();
        info.teacherNickName = query.value("teacherNickName").toString();

        //teacherCourseList
        info.date = query.value("date").toString();
        info.weekend = query.value("weekend").toString();
        info.studentName = query.value("studentName").toString();

        info.school = query.value("school").toString();
        info.studentPhoneNubmer = query.value("studentPhoneNubmer").toString();
        info.grade = query.value("grade").toString();
        info.suject = query.value("suject").toString();
        info.time = query.value("time").toString();
        info.learningType = query.value("learningType").toString();
        info.courseTime = query.value("courseTime").toString();
        info.studentFee = query.value("studentFee").toString();
        info.studentTotalFee = query.value("studentTotalFee").toString();
        info.teacherFee = query.value("teacherFee").toString();
        info.gotMoney = query.value("gotMoney").toString();
        info.payType = query.value("payType").toString();
        info.payDate = query.value("payDate").toString();

        infos.emplace_back(info);
        cout << cnt++ << info.teacherName.toStdString() << endl;
    }
    cout << "mytest3" << endl;
}
