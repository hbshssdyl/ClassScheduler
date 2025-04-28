#ifndef DBMANAGER_H
#define DBMANAGER_H
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
using DBManagerPtr = std::shared_ptr<class DBManager>;

class DBManager : public std::enable_shared_from_this<DBManager>
{
public:
    DBManager();
    virtual ~DBManager() = default;

public:
    bool createDBConnection();
    void storeAllTableDataCount();
    bool createTeacherInfosTable();
    void dropTable(QString tableName);
    bool insertDataToTeacherInfosTable(TeacherInfos& infos);
    void queryDataFromTeacherInfosTable(TeacherInfos& infos);
    bool isTableExist(QString tableName);
    int getTableDataCount(QString tableName);
    bool refreshDBDataByFile(QString filePath, bool inNewThread);

private:
    bool isUsefulHeader(QString header);
    bool hasValidHeaders(Document& doc);
    void saveData(TeacherInfo& info, QString& headerStr, QString& str);
    QVariant readCellValue(QString headerStr, CellPtr cell);
    TeacherInfos getTeacherInfosFromExcelFile(QString filePath);

private:
    QSqlDatabase mDB;
    TableDataCount mDataCount;

};

#endif // DBMANAGER_H
