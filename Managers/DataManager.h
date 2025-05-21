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
    void storeAllTableDataCount();
    int getTableDataCount(QString tableName);
    bool refreshDBDataByFile(QString filePath);
    void refreshAllDataFromDB();
    ClassInfos getClassInfosFromDB();
    TeacherInfos getTeacherInfosFromDB();
    TeacherStudentInfos getTeacherStudentInfosFromDB();

private:
    bool isUsefulHeader(QString header);
    bool hasValidHeaders(Document& doc);
    void saveData(ClassInfo& info, QString& headerStr, QString& str);
    QVariant readCellValue(QString headerStr, CellPtr cell);
    ClassInfos getClassInfosFromExcelFile(QString filePath);
    bool saveDataToClassInfosTable(ClassInfos& infos);
    bool saveDataToTeacherInfosTable(TeacherInfos& infos);
    TeacherInfos getTeacherInfosList(ClassInfos& classInfos);
    bool createClassInfosTable();
    bool createTeacherInfosTable();
    bool insertDataToClassInfosTable(ClassInfos& infos);
    bool insertDataToTeacherInfosTable(TeacherInfos& infos);
    void queryDataFromClassInfosTable(ClassInfos& infos);
    void queryDataFromTeacherInfosTable(TeacherInfos& infos);
    bool isTableExist(QString tableName);
    void dropTable(QString tableName);
    QString transToChinese(QString englishWeekday);
    QString formatTime(const QString& time);

    //teacherStudentInfos
    void generateTeacherStudentInfos();

private:
    QSqlDatabase mDB;
    ClassInfos mClassInfosFromDB;
    TeacherInfos mTeacherInfosFromDB;
    TeacherStudentInfos mTeacherStudentInfos;
    TableDataCount mDataCount;

};

#endif // DATAMANAGER_H
