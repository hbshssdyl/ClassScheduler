#include "DBManager.h"
#include <iostream>

using namespace ClassScheduler;

#define OPEN_DATABASE()                                    \
    mDB = QSqlDatabase::addDatabase("QSQLITE");            \
    mDB.setDatabaseName("ClassScheduler.db");

DBManager::DBManager() {}

bool DBManager::createDBConnection()
{
    OPEN_DATABASE()
    if (!mDB.open()) {
        cout << "Failed to connect database." << endl;
        return false;
    }
    return true;
}

int DBManager::getTableDataCount(QString tableName)
{
    if(mDataCount.count(tableName) == 1)
    {
        return mDataCount[tableName];
    }
    else
    {
        cout << "table is not exist, table name: " << tableName.toStdString() << endl;
    }
    return 0;
}

bool DBManager::createClassInfosTable()
{
    QSqlQuery query;
    bool ret = query.exec("CREATE TABLE classInfos("
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

void DBManager::dropTable(QString tableName)
{
    QSqlQuery query;
    bool ret = query.exec(QString("DROP TABLE '%1'").arg(tableName));
    if (!ret) {
        cout << "Failed to drop table: " << query.lastError().text().toStdString() << endl;
    }
}

bool DBManager::insertDataToClassInfosTable(ClassInfos& infos)
{
    for(auto& info : infos)
    {
        QSqlQuery query;
        QString sql = QString("INSERT INTO classInfos (date, weekend, studentName, school, studentPhoneNubmer, grade,"
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

bool DBManager::isTableExist(QString tableName)
{
    QSqlQuery query(mDB);
    query.exec(QString("select * from sqlite_master where type='table' and name='%1'").arg(tableName));
    return query.next();;
}

bool DBManager::refreshDBDataByFile(QString filePath, bool inNewThread)
{
    auto classInfos = getClassInfosFromExcelFile(filePath);
    if(inNewThread)
    {
        createDBConnection();
    }
    dropTable(TEACHER_INFOS_TABLE_NAME);
    if(createClassInfosTable())
    {
        if(insertDataToClassInfosTable(classInfos))
        {
            if(getTableDataCount(TEACHER_INFOS_TABLE_NAME) > 0)
            {
                return true;
            }
        }
    }
    return false;
}

void DBManager::queryDataFromClassInfosTable(ClassInfos& infos)
{
    QSqlQuery query("SELECT * FROM classInfos");
    int cnt = 1;
    while (query.next()) {
        ClassInfo info;
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
    }
}

void DBManager::storeAllTableDataCount()
{
    QSqlQuery query;
    for(auto name : allTableNameForDB)
    {
        query.exec(QString("SELECT count(*) FROM '%1'").arg(name));
        query.next();
        auto count = query.value(0).toInt();
        mDataCount[name] = count;
        cout << "All tableDataCount, " << name << ": " << count << endl;
    }
}

ClassInfos DBManager::getClassInfosFromExcelFile(QString filePath)
{
    ClassInfos infos;
    Document doc(filePath);
    if (!doc.load())
        return infos;

    if(!hasValidHeaders(doc))
    {
        return infos;
    }

    int row = 2; int col = 1;
    while(row)
    {
        col = 1;
        ClassInfo info;
        while(col)
        {
            CellPtr headerCell = doc.cellAt(1, col);
            if(!headerCell)
            {
                break;
            }
            QVariant header = headerCell->readValue();

            auto headerStr = header.toString();
            if(headerStr.isEmpty())
            {
                break;
            }
            if(!isUsefulHeader(headerStr))
            {
                col++;
                continue;
            }

            CellPtr cell = doc.cellAt(row, col);
            if(!cell)
            {
                col++;
                continue;
            }
            QVariant var = readCellValue(headerStr, cell);
            auto str = var.toString();

            saveData(info, headerStr, str);
            col++;
        }
        if(!info.isValidInfo())
        {
            break;
        }
        infos.emplace_back(info);
        row++;
    }
    return infos;
}

bool DBManager::hasValidHeaders(Document& doc)
{
    int row = 1, col = 1;
    map<QString, bool> headers;
    while(col)
    {
        CellPtr cell = doc.cellAt(row, col);
        if(!cell)
        {
            break;
        }
        QVariant var = cell->value();
        auto str = var.toString();
        //cout << str << endl;
        if(str.isEmpty())
        {
            break;
        }
        headers[str] = true;
        col++;
    }
    for(auto header : validExcelHeader)
    {
        if(!headers[header])
        {
            cout << "无该信息: " << header << endl;
            return false;
        }
    }
    return true;
}

void DBManager::saveData(ClassInfo& info, QString& headerStr, QString& str)
{
    if(str.isEmpty() || str == "00:00:00.000")
    {
        str = nullString;
    }
    if(headerStr == "日期") info.date = str;
    else if(headerStr == "星期") info.weekend = str;
    else if(headerStr == "姓名") info.studentName = str;
    else if(headerStr == "学校") info.school = str;
    else if(headerStr == "电话") info.studentPhoneNubmer = str;
    else if(headerStr == "年级") info.grade = str;
    else if(headerStr == "学科") info.suject = str;
    else if(headerStr == "时间") info.time = str;
    else if(headerStr == "老师") info.teacherNickName = str;
    else if(headerStr == "网课or面授") info.learningType = str;
    else if(headerStr == "课时") info.courseTime = str;
    else if(headerStr == "金额/小时") info.studentFee = str;
    else if(headerStr == "课酬总计") info.studentTotalFee = str;
    else if(headerStr == "老师姓名") info.teacherName = str;
    else if(headerStr == "老师工资") info.teacherFee = str;
    else if(headerStr == "已收金额") info.gotMoney = str;
    else if(headerStr == "付费方式") info.payType = str;
    else if(headerStr == "收费日期") info.payDate = str;
}

bool DBManager::isUsefulHeader(QString header)
{
    for(auto str : validExcelHeader)
    {
        if(header == str) return true;
    }
    return false;
}

QVariant DBManager::readCellValue(QString headerStr, CellPtr cell)
{
    if(headerStr == "星期")
    {
        return cell->value();
    }
    return cell->readValue();
}
