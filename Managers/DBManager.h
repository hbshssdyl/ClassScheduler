#ifndef DBMANAGER_H
#define DBMANAGER_H
#include "Utils/DataUtils.h"
#include <memory>

using namespace ClassScheduler;
using DBManagerPtr = std::shared_ptr<class DBManager>;

class DBManager : public std::enable_shared_from_this<DBManager>
{
public:
    DBManager();
    virtual ~DBManager() = default;

public:
    bool createDBConnection();
    bool createTeacherInfosTable();
    bool insertDataToTeacherInfosTable(TeacherInfos& infos);
    void queryDataFromTeacherInfosTable(TeacherInfos& infos);

};

#endif // DBMANAGER_H
