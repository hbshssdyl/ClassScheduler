#include "DataManager.h"
#include <iostream>

using namespace ClassScheduler;

#define OPEN_DATABASE()                                    \
    mDB = QSqlDatabase::addDatabase("QSQLITE");            \
    mDB.setDatabaseName("ClassScheduler.db");

DataManager::DataManager() {}

bool DataManager::createDBConnection()
{
    OPEN_DATABASE()
    if (!mDB.open()) {
        cout << "Failed to connect database." << endl;
        return false;
    }
    return true;
}

int DataManager::getTableDataCount(QString tableName)
{
    if(mDataCount.count(tableName) == 1)
    {
        cout << "table is exist, table name: " << tableName.toStdString() << ", count: " << mDataCount[tableName] << endl;
        return mDataCount[tableName];
    }
    else
    {
        cout << "table is not exist, table name: " << tableName.toStdString() << endl;
    }
    return 0;
}

bool DataManager::createClassInfosTable()
{
    QSqlQuery query;
    bool ret = query.exec("CREATE TABLE classInfos("
                          "id                 INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "date               TEXT,"
                          "weekend            TEXT            NOT NULL,"
                          "studentName        TEXT            NOT NULL,"
                          "school             TEXT,"
                          "studentPhoneNubmer TEXT,"
                          "grade              TEXT,"
                          "suject             TEXT,"
                          "time               TEXT            NOT NULL,"
                          "teacherNickName    TEXT            NOT NULL,"
                          "learningType       TEXT            NOT NULL,"
                          "courseTime         TEXT,"
                          "studentFee         TEXT            NOT NULL,"
                          "studentTotalFee    TEXT,"
                          "teacherName        TEXT,"
                          "teacherFee         TEXT,"
                          "gotMoney           TEXT,"
                          "payType            TEXT,"
                          "payDate            TEXT"
                          ")");
    if (!ret) {
        cout << "Failed to create table: " << query.lastError().text().toStdString() << endl;
        return false;
    }
    return true;
}

bool DataManager::createTeacherInfosTable()
{
    QSqlQuery query;
    bool ret = query.exec("CREATE TABLE teacherInfos("
                          "id                        INTEGER  PRIMARY KEY   AUTOINCREMENT,"
                          "teacherName               TEXT                   NOT NULL,"
                          "teacherNickNames          TEXT                   NOT NULL,"
                          "teacherSujectsAndFees     TEXT                   NOT NULL,"
                          "teacherSujectsAndGrades   TEXT"
                          ")");
    if (!ret) {
        cout << "Failed to create table: " << query.lastError().text().toStdString() << endl;
        return false;
    }
    return true;
}

void DataManager::dropTable(QString tableName)
{
    QSqlQuery query;
    bool ret = query.exec(QString("DROP TABLE '%1'").arg(tableName));
    if (!ret) {
        cout << "Failed to drop table: " << query.lastError().text().toStdString() << endl;
    }
}

bool DataManager::insertDataToClassInfosTable(ClassInfos& infos)
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
            cout << "Failed to insert data to classInfos: " << query.lastError().text().toStdString() << endl;
            return false;
        }
    }
    return true;
}

bool DataManager::insertDataToTeacherInfosTable(TeacherInfos& infos)
{
    for(auto& info : infos)
    {
        QSqlQuery query;
        QString sql = QString("INSERT INTO teacherInfos (teacherName, teacherNickNames, teacherSujectsAndFees, teacherSujectsAndGrades)"
                              "VALUES ('%1', '%2', '%3', '%4')")
                          .arg(info.teacherName).arg(info.getTeacherNickNames()).arg(info.getTeacherSujectsAndFees()).arg(info.getTeacherSujectsAndGrades());
        bool ret = query.exec(sql);
        if (!ret) {
            cout << "Failed to insert data to teacherInfos: " << query.lastError().text().toStdString() << endl;
            return false;
        }
    }
    return true;
}

bool DataManager::isTableExist(QString tableName)
{
    QSqlQuery query(mDB);
    query.exec(QString("select * from sqlite_master where type='table' and name='%1'").arg(tableName));
    return query.next();;
}

bool DataManager::refreshDBDataByFile(QString filePath)
{
    auto classInfos = getClassInfosFromExcelFile(filePath);

    if(mDataCount[CLASS_INFOS_TABLE_NAME] > 0)
    {
        cout << "drop table: " << CLASS_INFOS_TABLE_NAME.toStdString() << endl;
        dropTable(CLASS_INFOS_TABLE_NAME);
    }

    if(mDataCount[TEACHER_INFOS_TABLE_NAME] > 0)
    {
        cout << "drop table: " << TEACHER_INFOS_TABLE_NAME.toStdString() << endl;
        dropTable(TEACHER_INFOS_TABLE_NAME);
    }

    if(!saveDataToClassInfosTable(classInfos))
    {
        return false;
    }
    auto teacherInfos = getTeacherInfosList(classInfos);
    if(!saveDataToTeacherInfosTable(teacherInfos))
    {
        return false;
    }

    storeAllTableDataCount();
    if(getTableDataCount(CLASS_INFOS_TABLE_NAME) == 0 || getTableDataCount(TEACHER_INFOS_TABLE_NAME) == 0)
    {
        return false;
    }

    refreshAllDataFromDB();

    return true;
}

void DataManager::refreshAllDataFromDB()
{
    queryDataFromClassInfosTable(mClassInfosFromDB);
    queryDataFromTeacherInfosTable(mTeacherInfosFromDB);
    generateTeacherStudentInfos();
}

ClassInfos DataManager::getClassInfosFromDB()
{
    return mClassInfosFromDB;
}

TeacherInfos DataManager::getTeacherInfosFromDB()
{
    return mTeacherInfosFromDB;
}

TeacherStudentInfos DataManager::getTeacherStudentInfosFromDB()
{
    return mTeacherStudentInfos;
}

bool DataManager::saveDataToClassInfosTable(ClassInfos& infos)
{
    if(createClassInfosTable())
    {
        if(insertDataToClassInfosTable(infos))
        {
            return true;
        }
    }
    return false;
}

bool DataManager::saveDataToTeacherInfosTable(TeacherInfos& infos)
{
    if(createTeacherInfosTable())
    {
        if(insertDataToTeacherInfosTable(infos))
        {
            return true;
        }
    }
    return false;
}

TeacherInfos DataManager::getTeacherInfosList(ClassInfos& classInfos)
{
    TeacherInfos teacherInfos;

    //save teacherName
    for(auto& classInfo : classInfos)
    {
        auto teacherName = classInfo.teacherName;
        bool save = true;
        for(auto& teacherInfo : teacherInfos)
        {
            if(teacherInfo.teacherName == teacherName)
            {
                save = false;
                break;
            }
        }
        if(save)
        {
            teacherInfos.emplace_back(TeacherInfo(teacherName));
        }
    }

    //save teacherNickNames, teacherFees, teacherSujects, teacherGrades
    for(auto& classInfo : classInfos)
    {
        auto teacherName = classInfo.teacherName;
        auto teacherNickName = classInfo.teacherNickName;
        auto teacherSuject = classInfo.suject;
        auto teacherSujectAndFee = teacherSuject + "_" + classInfo.teacherFee;
        auto teacherSujectAndGrade = teacherSuject + "_" + classInfo.grade;
        for(auto& teacherInfo : teacherInfos)
        {
            if(teacherInfo.teacherName == teacherName)
            {
                teacherInfo.saveValue(teacherNickName, teacherInfo.teacherNickNames);
                teacherInfo.saveValue(teacherSujectAndFee, teacherInfo.teacherSujectsAndFees);
                teacherInfo.saveValue(teacherSujectAndGrade, teacherInfo.teacherSujectsAndGrades);
                break;
            }
        }
    }
    //sort teacherInfos
    for(auto& teacherInfo : teacherInfos)
    {
        teacherInfo.sortInfos();
    }
    return teacherInfos;
}

void DataManager::queryDataFromClassInfosTable(ClassInfos& infos)
{
    QSqlQuery query("SELECT * FROM classInfos");
    int cnt = 1;
    while (query.next()) {
        ClassInfo info;

        info.teacherName = query.value("teacherName").toString();
        info.teacherNickName = query.value("teacherNickName").toString();
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

void DataManager::queryDataFromTeacherInfosTable(TeacherInfos& infos)
{
    QSqlQuery query("SELECT * FROM teacherInfos");
    int cnt = 1;
    while (query.next()) {
        TeacherInfo info;

        info.teacherName = query.value("teacherName").toString();
        info.strTeacherNickNames = query.value("teacherNickNames").toString();
        info.strTeacherSujectsAndFees = query.value("teacherSujectsAndFees").toString();
        info.strTeacherSujectsAndGrades = query.value("teacherSujectsAndGrades").toString();

        infos.emplace_back(info);
    }
}

void DataManager::storeAllTableDataCount()
{
    QSqlQuery query;
    for(auto name : allTableNameForDB)
    {
        if(!isTableExist(name))
        {
            cout << "Table " << name << " is not exist" << endl;
        }
        query.exec(QString("SELECT count(*) FROM '%1'").arg(name));
        query.next();
        auto count = query.value(0).toInt();
        mDataCount[name] = count;
        cout << "All tableDataCount, " << name << ": " << count << endl;
    }
}

ClassInfos DataManager::getClassInfosFromExcelFile(QString filePath)
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

bool DataManager::hasValidHeaders(Document& doc)
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
    for(auto header : validExcelClassHeader)
    {
        if(!headers[header])
        {
            cout << "无该信息: " << header << endl;
            return false;
        }
    }
    return true;
}

QString DataManager::formatTime(const QString& timeRange) {
    QStringList rangeParts = timeRange.split('-');
    if (rangeParts.size() != 2) {
        return ""; // 返回空字符串表示格式错误
    }

    auto formatTime = [](const QString& time) -> QString {
        QStringList parts = time.split(':');
        if (parts.size() != 2) {
            return ""; // 返回空字符串表示格式错误
        }

        int hour = parts[0].toInt();
        int minute = parts[1].toInt();

        return QString("%1:%2")
            .arg(hour, 2, 10, QChar('0'))
            .arg(minute, 2, 10, QChar('0'));
    };

    QString startTime = formatTime(rangeParts[0]);
    QString endTime = formatTime(rangeParts[1]);

    if (startTime.isEmpty() || endTime.isEmpty()) {
        return ""; // 返回空字符串表示格式错误
    }

    return (startTime + "-" + endTime);
}

QString DataManager::transToChinese(QString englishWeekday)
{
    QString day = englishWeekday.toLower();
    day[0] = day[0].toUpper();
    if (day == "Monday") return "星期一";
    if (day == "Tuesday") return "星期二";
    if (day == "Wednesday") return "星期三";
    if (day == "Thursday") return "星期四";
    if (day == "Friday") return "星期五";
    if (day == "Saturday") return "星期六";
    if (day == "Sunday") return "星期日";
    return englishWeekday;
}

void DataManager::saveData(ClassInfo& info, QString& headerStr, QString& str)
{
    if(str.isEmpty() || str == "00:00:00.000")
    {
        str = nullString;
    }
    if(headerStr == "日期") info.date = str;
    else if(headerStr == "星期") info.weekend = transToChinese(str);
    else if(headerStr == "姓名") info.studentName = str;
    else if(headerStr == "学校") info.school = str;
    else if(headerStr == "电话") info.studentPhoneNubmer = str;
    else if(headerStr == "年级") info.grade = str;
    else if(headerStr == "学科") info.suject = str;
    else if(headerStr == "时间") info.time = formatTime(str);
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

void DataManager::generateTeacherStudentInfos()
{
    for(auto& classInfo : mClassInfosFromDB)
    {
        bool isNewTeacher = true;
        for(auto& info : mTeacherStudentInfos)
        {
            if(classInfo.teacherName == info.teacherName)
            {
                isNewTeacher = false;
                info.addInfo(classInfo.suject, classInfo.date, classInfo.studentName);
                break;
            }
        }
        if(isNewTeacher)
        {
            TeacherStudentInfo teacherStudentInfo(classInfo.teacherName, classInfo.suject, classInfo.date, classInfo.studentName);
            mTeacherStudentInfos.emplace_back(teacherStudentInfo);
        }
    }
    for(auto& teacherStudentInfo : mTeacherStudentInfos)
    {
        for(auto& sujectStudentInfo : teacherStudentInfo.sujectStudentInfos)
        {
            sujectStudentInfo.sortMonthStudentInfosByYearMonth();
        }
    }
}

bool DataManager::isUsefulHeader(QString header)
{
    for(auto str : validExcelClassHeader)
    {
        if(header == str) return true;
    }
    return false;
}

QVariant DataManager::readCellValue(QString headerStr, CellPtr cell)
{
    if(headerStr == "星期")
    {
        return cell->value();
    }
    return cell->readValue();
}
