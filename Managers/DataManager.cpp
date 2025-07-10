#include "DataManager.h"
#include <iostream>

using namespace ClassScheduler;

DataManager::DataManager() {}

bool DataManager::createDBConnection()
{
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("ClassScheduler.db");
    if (!db.open()) {
        cout << "Failed to connect database." << endl;
        return false;
    }
    return true;
}

bool DataManager::init()
{
    bool ret = true;

    if(!isTableExist(APP_TOGGLE_INFOS_TABLE_NAME))
    {
        ret &= createAppSettingsTable();
        ret &= initializeAppSettings();
    }

    readAllSettings();
    readAllUserInfos();

    cout << "DataManager init: " << ret << endl;
    return ret;
}

void DataManager::closeDBConnection()
{
    QString connectionName = QSqlDatabase::database().connectionName();
    mDB.close();
    QSqlDatabase::removeDatabase(connectionName);
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
                          "teacherSujectsAndStudents TEXT                   NOT NULL,"
                          "teacherSujectsAndFees     TEXT                   NOT NULL,"
                          "teacherSujectsAndGrades   TEXT                   NOT NULL"
                          ")");
    if (!ret) {
        cout << "Failed to create table: " << query.lastError().text().toStdString() << endl;
        return false;
    }
    return true;
}

bool DataManager::createStudentInfosTable()
{
    QSqlQuery query;
    bool ret = query.exec("CREATE TABLE studentInfos("
                          "id                        INTEGER  PRIMARY KEY   AUTOINCREMENT,"
                          "studentName               TEXT                   NOT NULL,"
                          "studentSchools            TEXT                   NOT NULL,"
                          "studentPhoneNumbers       TEXT                   NOT NULL,"
                          "studentTeachers           TEXT                   NOT NULL,"
                          "studentSujectsAndPays     TEXT                   NOT NULL"
                          ")");
    if (!ret) {
        cout << "Failed to create table: " << query.lastError().text().toStdString() << endl;
        return false;
    }
    return true;
}

bool DataManager::createAppSettingsTable()
{
    QSqlQuery query;
    QString createSql =
        "CREATE TABLE IF NOT EXISTS settings ("
        "key TEXT PRIMARY KEY,"
        "value INTEGER NOT NULL)";

    if (!query.exec(createSql)) {
        qWarning() << "Failed to create settings table:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DataManager::initializeAppSettings()
{
    QSqlQuery query;

    query.prepare("INSERT OR REPLACE INTO settings (key, value) VALUES (:key, :value)");

    for (const auto& setting : initialAppSettings) {
        query.bindValue(":key", setting.key);
        query.bindValue(":value", setting.value ? 1 : 0);
        if (!query.exec()) {
            qWarning() << "Failed to insert setting:" << setting.key << query.lastError().text();
            return false;
        }
    }

    return true;
}

bool DataManager::setAppSetting(const QString& key, bool value)
{
    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO settings (key, value) VALUES (:key, :value)");
    query.bindValue(":key", key);
    query.bindValue(":value", value ? 1 : 0);

    if (!query.exec()) {
        qWarning() << "Failed to update setting:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DataManager::readAllSettings()
{
    mAppSettings.clear();

    QSqlQuery query("SELECT key, value FROM settings");
    if (!query.exec()) {
        qWarning() << "Failed to load settings:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        Setting s;
        s.key = query.value(0).toString();
        s.value = query.value(1).toInt() != 0;  // 1 = true, 0 = false
        mAppSettings.push_back(s);
    }

    return true;
}

bool DataManager::readAllUserInfos()
{
    mUserInfos.emplace_back(UserInfo("zhuhui", "zh615", UserLevel::Owner));
    mUserInfos.emplace_back(UserInfo("tingting", "xyt123", UserLevel::RegularUser));
    mUserInfos.emplace_back(UserInfo("xiaoru", "cmr123", UserLevel::Owner));
    mUserInfos.emplace_back(UserInfo("Dylan", "dylanadmin", UserLevel::SuperAdmin));
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
        QString sql = QString("INSERT INTO teacherInfos (teacherName, teacherNickNames, teacherSujectsAndStudents, teacherSujectsAndFees, teacherSujectsAndGrades)"
                              "VALUES ('%1', '%2', '%3', '%4', '%5')")
                          .arg(info.teacherName).arg(info.getTeacherNickNames()).arg(info.getTeacherSujectsAndStudents()).arg(info.getTeacherSujectsAndFees()).arg(info.getTeacherSujectsAndGrades());
        bool ret = query.exec(sql);
        if (!ret) {
            cout << "Failed to insert data to teacherInfos: " << query.lastError().text().toStdString() << endl;
            return false;
        }
    }
    return true;
}

bool DataManager::insertDataToStudentInfosTable(StudentInfos& infos)
{
    for(auto& info : infos)
    {
        QSqlQuery query;
        QString sql = QString("INSERT INTO studentInfos (studentName, studentSchools, studentPhoneNumbers, studentTeachers, studentSujectsAndPays)"
                              "VALUES ('%1', '%2', '%3', '%4', '%5')")
                          .arg(info.studentName).arg(info.getStudentSchools()).arg(info.getStudentPhoneNumbers()).arg(info.getStudentTeachers()).arg(info.getStudentSujectsAndPays());
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
    QSqlQuery query;
    query.exec(QString("select * from sqlite_master where type='table' and name='%1'").arg(tableName));
    return query.next();;
}

bool DataManager::refreshAllDataFromFile(QString filePath)
{
    clearAllData();
    mClassInfosFromDB = getClassInfosFromExcelFile(filePath);
    mTeacherInfosFromDB = getTeacherInfosList(mClassInfosFromDB);
    mStudentInfosFromDB = getStudentInfosList(mClassInfosFromDB);
    generateTeacherStudentInfos();
    generateStudentClassInfos();

    if(isTableExist(CLASS_INFOS_TABLE_NAME))
    {
        cout << "drop table: " << CLASS_INFOS_TABLE_NAME.toStdString() << endl;
        dropTable(CLASS_INFOS_TABLE_NAME);
    }

    if(isTableExist(TEACHER_INFOS_TABLE_NAME))
    {
        cout << "drop table: " << TEACHER_INFOS_TABLE_NAME.toStdString() << endl;
        dropTable(TEACHER_INFOS_TABLE_NAME);
    }

    if(isTableExist(STUDENT_INFOS_TABLE_NAME))
    {
        cout << "drop table: " << STUDENT_INFOS_TABLE_NAME.toStdString() << endl;
        dropTable(STUDENT_INFOS_TABLE_NAME);
    }

    if(!saveDataToClassInfosTable(mClassInfosFromDB))
    {
        cout << "Failed to save data to ClassInfos table" << endl;
        return false;
    }

    if(!saveDataToTeacherInfosTable(mTeacherInfosFromDB))
    {
        cout << "Failed to save data to TeacherInfos table" << endl;
        return false;
    }

    if(!saveDataToStudentInfosTable(mStudentInfosFromDB))
    {
        cout << "Failed to save data to StudentInfos table" << endl;
        return false;
    }

    storeAllTableDataCount();
    if(getTableDataCount(CLASS_INFOS_TABLE_NAME) == 0 ||
       getTableDataCount(TEACHER_INFOS_TABLE_NAME) == 0 ||
       getTableDataCount(STUDENT_INFOS_TABLE_NAME) == 0)
    {
        return false;
    }

    refreshAllDataFromDB();

    return true;
}

void DataManager::clearAllData()
{
    mClassInfosFromDB.clear();
    mTeacherInfosFromDB.clear();
    mStudentInfosFromDB.clear();
}

void DataManager::refreshAllDataFromDB()
{
    clearAllData();
    queryDataFromClassInfosTable(mClassInfosFromDB);
    queryDataFromTeacherInfosTable(mTeacherInfosFromDB);
    queryDataFromStudentInfosTable(mStudentInfosFromDB);
}

ClassInfos DataManager::getClassInfosFromDB()
{
    return mClassInfosFromDB;
}

TeacherInfos DataManager::getTeacherInfosFromDB()
{
    return mTeacherInfosFromDB;
}

StudentInfos DataManager::getStudentInfosFromDB()
{
    return mStudentInfosFromDB;
}

AppSettings DataManager::getAppSettingsFromDB()
{
    return mAppSettings;
}

UserInfos DataManager::getUserInfoFromDB()
{
    return mUserInfos;
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

bool DataManager::saveDataToStudentInfosTable(StudentInfos& infos)
{
    if(createStudentInfosTable())
    {
        if(insertDataToStudentInfosTable(infos))
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
        auto teacherSujectAndStudent = teacherSuject + "_" + classInfo.studentName;
        auto teacherSujectAndGrade = teacherSuject + "_" + classInfo.grade;
        for(auto& teacherInfo : teacherInfos)
        {
            if(teacherInfo.teacherName == teacherName)
            {
                teacherInfo.saveValue(teacherNickName, teacherInfo.teacherNickNames);
                teacherInfo.saveValue(teacherSujectAndStudent, teacherInfo.teacherSujectsAndStudents);
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

StudentInfos DataManager::getStudentInfosList(ClassInfos& classInfos)
{
    StudentInfos studentInfos;

    //save studentName
    for(auto& classInfo : classInfos)
    {
        auto studentName = classInfo.studentName;
        bool save = true;
        for(auto& studentInfo : studentInfos)
        {
            if(studentInfo.studentName == studentName)
            {
                save = false;
                break;
            }
        }
        if(save)
        {
            studentInfos.emplace_back(StudentInfo(studentName));
        }
    }

    //save studentSchools, studentPhoneNumbers, studentTeachers - 张三（张三昵称）, studentSujectsAndPays - 物理： 100(张三)，200（李四）
    for(auto& classInfo : classInfos)
    {
        auto studentName = classInfo.studentName;
        auto studentSchool = classInfo.school;
        auto studentPhoneNumber = classInfo.studentPhoneNubmer;
        auto studentTeacher = classInfo.suject + "_" + classInfo.teacherName + "（" + classInfo.teacherNickName + "）";
        auto studentSujectsAndPay = classInfo.suject + "_" + classInfo.studentFee + "（" + classInfo.teacherName + "）";
        for(auto& studentInfo : studentInfos)
        {
            if(studentInfo.studentName == studentName)
            {
                studentInfo.saveValue(studentSchool, studentInfo.studentSchools);
                studentInfo.saveValue(studentPhoneNumber, studentInfo.studentPhoneNumbers);
                studentInfo.saveValue(studentTeacher, studentInfo.studentTeachers);
                studentInfo.saveValue(studentSujectsAndPay, studentInfo.studentSujectsAndPays);
                break;
            }
        }
    }
    //sort studentInfos
    for(auto& studentInfo : studentInfos)
    {
        studentInfo.sortInfos();
    }
    return studentInfos;
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
        info.strTeacherSujectsAndStudents = query.value("teacherSujectsAndStudents").toString();
        info.strTeacherSujectsAndFees = query.value("teacherSujectsAndFees").toString();
        info.strTeacherSujectsAndGrades = query.value("teacherSujectsAndGrades").toString();

        infos.emplace_back(info);
    }
    generateTeacherStudentInfos();
}

void DataManager::queryDataFromStudentInfosTable(StudentInfos& infos)
{
    QSqlQuery query("SELECT * FROM studentInfos");
    int cnt = 1;
    while (query.next()) {
        StudentInfo info;

        info.studentName = query.value("studentName").toString();
        info.strStudentSchools = query.value("studentSchools").toString();
        info.strStudentPhoneNumbers = query.value("studentPhoneNumbers").toString();
        info.strStudentTeachers = query.value("studentTeachers").toString();
        info.strStudentSujectsAndPays = query.value("studentSujectsAndPays").toString();

        infos.emplace_back(info);
    }
    generateStudentClassInfos();
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

void DataManager::generateTeacherStudentBasicInfo()
{
    for(auto& teacherInfo : mTeacherInfosFromDB)
    {
        for(auto& sujectStudentInfo : teacherInfo.sujectStudentCounts)
        {
            for(auto& monthStudentInfo : sujectStudentInfo.monthCountInfos)
            {
                mTeacherInfosFromDB[0].teacherStudentCountBasicInfo.refreshData(monthStudentInfo.yearMonth, monthStudentInfo.keyCount);
            }
        }
    }
}

void DataManager::generateTeacherStudentInfos()
{
    for(auto& classInfo : mClassInfosFromDB)
    {
        bool isNewTeacher = true;
        for(auto& info : mTeacherInfosFromDB)
        {
            if(classInfo.teacherName == info.teacherName)
            {
                isNewTeacher = false;
                info.addSujectCountInfo("总", classInfo.date, classInfo.suject + "_" + classInfo.studentName);
                info.addSujectCountInfo(classInfo.suject, classInfo.date, classInfo.studentName);
                break;
            }
        }
        if(isNewTeacher)
        {
            cout << "Error: there should not be a new teacher" << endl;
        }
    }

    generateTeacherStudentBasicInfo();
    if(mTeacherInfosFromDB.size() == 0)
    {
        return;
    }

    auto minYearMonth = mTeacherInfosFromDB[0].teacherStudentCountBasicInfo.minYearMonth;
    auto maxYearMonth = mTeacherInfosFromDB[0].teacherStudentCountBasicInfo.maxYearMonth;
    for(auto& info : mTeacherInfosFromDB)
    {
        for(auto& sujectStudentCount : info.sujectStudentCounts)
        {
            sujectStudentCount.sortMonthStudentInfosByYearMonth();
            sujectStudentCount.fillMissingMonths(minYearMonth, maxYearMonth);
        }
    }
}

void DataManager::generateStudentClassBasicInfo()
{
    for(auto& studentInfo : mStudentInfosFromDB)
    {
        studentInfo.studentClassCountBasicInfo.showTotal = true;
        studentInfo.studentClassCountBasicInfo.showTotal = true;
        for(auto& sujectClassInfo : studentInfo.sujectClassCounts)
        {
            for(auto& monthClassInfo : sujectClassInfo.monthCountInfos)
            {
                mStudentInfosFromDB[0].studentClassCountBasicInfo.refreshData(monthClassInfo.yearMonth, monthClassInfo.keyCount);
            }
        }
    }
}

void DataManager::generateStudentClassInfos()
{
    for(auto& classInfo : mClassInfosFromDB)
    {
        bool isNewStudent = true;
        for(auto& info : mStudentInfosFromDB)
        {
            if(classInfo.studentName == info.studentName)
            {
                isNewStudent = false;
                info.addSujectCountInfo("总", classInfo.date, classInfo.suject + "_" + classInfo.grade);
                info.addSujectCountInfo(classInfo.suject, classInfo.date, classInfo.grade);
               break;
            }
        }
        if(isNewStudent)
        {
            cout << "Error: there should not be a new student" << endl;
        }
    }

    generateStudentClassBasicInfo();
    if(mStudentInfosFromDB.size() == 0)
    {
        return;
    }

    auto minYearMonth = mStudentInfosFromDB[0].studentClassCountBasicInfo.minYearMonth;
    auto maxYearMonth = mStudentInfosFromDB[0].studentClassCountBasicInfo.maxYearMonth;
    for(auto& info : mStudentInfosFromDB)
    {
        for(auto& sujectClassCount : info.sujectClassCounts)
        {
            sujectClassCount.sortMonthStudentInfosByYearMonth();
            sujectClassCount.fillMissingMonths(minYearMonth, maxYearMonth);
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
