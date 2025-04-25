#pragma once

#include <QCoreApplication>
#include <thread>
#include <iostream>

#include "SearchTeacherInfoController.h"
#include "Utils/ControllerUtils.h"
#include "Managers/DBManager.h"

using namespace ClassScheduler;
const QString TEACHER_INFOS_TABLE_NAME = "teacherInfos";

SearchTeacherInfoController::SearchTeacherInfoController(DBManagerPtr dbManager, QObject* parent)
    : mDBManager(dbManager)
    , QObject(parent)
{
}

void SearchTeacherInfoController::initialize()
{
    dataInit();
    refreshSearchTeacherInfo();
}

void SearchTeacherInfoController::dataInit()
{
    if(mDBManager->isTableExist(TEACHER_INFOS_TABLE_NAME))
    {
        cout << "DB is exist" << endl;
        mIsDBDataExist = true;
    }
    else
    {
        cout << "init Teacher Infos From Excel File" << endl;
        initTeacherInfosFromExcelFile("test.xlsx");
    }
}

void SearchTeacherInfoController::initTeacherInfosFromExcelFile(QString filePath)
{
    if(!mDBManager)
    {
        cout << "mDBManager is null" << endl;
        return;
    }

    auto f = [this, filePath] {
        //std::unique_lock<std::mutex> u_lk(mTeacherInfosMutex);
        auto teacherInfos = CUtils::getTeacherInfosFromExcelFile(filePath);
        if(mDBManager->createDBConnection())
        {
            if(mDBManager->createTeacherInfosTable())
            {
                if(mDBManager->insertDataToTeacherInfosTable(teacherInfos))
                {
                    cout << "All datas have been set" << endl;
                    readyForTeacherInfos();
                }
            }
        }
    };

    std::thread t1(f);
    t1.detach();
}

void SearchTeacherInfoController::refreshSearchTeacherInfo()
{
    if(!mIsDBDataExist)
    {
        return;
    }

    initTeahcerHeader();
    readTeacherInfosFromDB();

    if(mTeacherInfosFromDB.size() == 0)
    {
        cout << "Fail to get teacher infos from DB" << endl;
        return;
    }

    QVariantList newTeacherInfoList;
    CUtils::updateTeacherInfoList(newTeacherInfoList, mTeacherInfosFromDB);

    if (mTeacherInfoList != newTeacherInfoList)
    {
        mTeacherInfoList = std::move(newTeacherInfoList);
        emit teacherInfoListChanged();
    }
}

void SearchTeacherInfoController::initTeahcerHeader()
{
    QVariantList newTeacherrHeaderList;
    CUtils::updateTeacherHeaderList(newTeacherrHeaderList);

    if (mTeacherHeaderList != newTeacherrHeaderList)
    {
        mTeacherHeaderList = std::move(newTeacherrHeaderList);
        emit teacherInfoHeaderChanged();
    }
}

void SearchTeacherInfoController::readTeacherInfosFromDB()
{
    if(mDBManager)
    {
        mDBManager->queryDataFromTeacherInfosTable(mTeacherInfosFromDB);
    }

    if(mTeacherInfosFromDB.size() == 0)
    {
        cout << "Fail to read teacher infos from DB" << endl;
    }
}

void SearchTeacherInfoController::readyForTeacherInfos()
{
    mIsDBDataExist = true;
    refreshSearchTeacherInfo();
}











































