#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include "Utils/DataUtils.h"
#include "xlsxdocument.h"
// #include "xlsxchartsheet.h"
// #include "xlsxcellrange.h"
// #include "xlsxchart.h"
// #include "xlsxrichstring.h"
#include "xlsxworkbook.h"

#include <QtSql>
#include <memory>

using namespace ClassScheduler;
using namespace QXlsx;
using CellPtr = std::shared_ptr<Cell>;
using DataManagerPtr = std::shared_ptr<class DataManager>;

class DataManager : public std::enable_shared_from_this<DataManager>
{
public:
    DataManager();
    virtual ~DataManager() = default;

public:
    bool createDBConnection();
    void closeDBConnection();
    void storeAllTableDataCount();
    int getTableDataCount(QString tableName);
    bool refreshAllDataFromFile(QString filePath);
    void refreshAllDataFromDB();
    ClassInfos getClassInfosFromDB();
    TeacherInfos getTeacherInfosFromDB();
    StudentInfos getStudentInfosFromDB();
    AppSettings getAppSettingsFromDB();
    LoginInfos getLoginInfoFromDB();

private:
    bool init();
    bool isUsefulHeader(QString header);
    bool hasValidHeaders(Document& doc);
    void clearAllData();
    void saveData(ClassInfo& info, QString& headerStr, QString& str);
    QVariant readCellValue(QString headerStr, CellPtr cell);
    ClassInfos getClassInfosFromExcelFile(QString filePath);
    bool saveDataToClassInfosTable(ClassInfos& infos);
    bool saveDataToTeacherInfosTable(TeacherInfos& infos);
    bool saveDataToStudentInfosTable(StudentInfos& infos);
    TeacherInfos getTeacherInfosList(ClassInfos& classInfos);
    StudentInfos getStudentInfosList(ClassInfos& classInfos);
    bool createClassInfosTable();
    bool createTeacherInfosTable();
    bool createStudentInfosTable();
    bool createAppSettingsTable();
    bool initializeAppSettings();
    bool readAllSettings();
    bool readAllUserInfos();
    bool setAppSetting(const QString& key, bool value);
    bool insertDataToClassInfosTable(ClassInfos& infos);
    bool insertDataToTeacherInfosTable(TeacherInfos& infos);
    bool insertDataToStudentInfosTable(StudentInfos& infos);
    void queryDataFromClassInfosTable(ClassInfos& infos);
    void queryDataFromTeacherInfosTable(TeacherInfos& infos);
    void queryDataFromStudentInfosTable(StudentInfos& infos);
    bool isTableExist(QString tableName);
    void dropTable(QString tableName);
    QString transToChinese(QString englishWeekday);
    QString formatTime(const QString& time);

    //teacherStudentInfos
    void generateTeacherStudentInfos();
    void generateTeacherStudentBasicInfo();

    //studentClassInfos
    void generateStudentClassInfos();
    void generateStudentClassBasicInfo();

private:
    QSqlDatabase mDB;
    ClassInfos mClassInfosFromDB;
    TeacherInfos mTeacherInfosFromDB;
    StudentInfos mStudentInfosFromDB;
    TableDataCount mDataCount;
    AppSettings mAppSettings;
    LoginInfos mLoginInfos;
};

#endif // DATAMANAGER_H
