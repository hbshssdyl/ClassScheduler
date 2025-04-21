#pragma once

#include <QCoreApplication>
#include "SearchTeacherInfoController.h"
#include "Utils/ControllerUtils.h"


#include "xlsxdocument.h"
// #include "xlsxchartsheet.h"
// #include "xlsxcellrange.h"
// #include "xlsxchart.h"
// #include "xlsxrichstring.h"

#include "xlsxworkbook.h"
using namespace QXlsx;

using namespace ClassScheduler;

SearchTeacherInfoController::SearchTeacherInfoController(QObject* parent)
    : QObject(parent)
{
    Document doc("FinalTest.xlsx");
    if (!doc.load())
        return;
    // QXlsx::CellRange range;
    // range = doc.dimension();
    // int rowCount = range.rowCount();
    // int colCount = range.columnCount();
    // cout << rowCount << " " << colCount << endl;

    // QVariant var1 = doc.read( 2409, 1 );
    // auto str = var1.toString().toStdString();

    // cout << "str: " << str << endl;

    int row = 1; int col = 1;
    while(row)
    {
        col = 1;
        while(col)
        {
            QVariant var = doc.read( row, col );
            auto str = var.toString().toStdString();
            //cout << row << " " << col << " " << str << endl;

            if(str == "")
                break;
            col++;
        }
        QVariant var1 = doc.read( row, 1 );
        auto str = var1.toString().toStdString();
        if(str == "")
            break;
        row++;
    }
cout << row << " " << col << endl;
    //validExcelHeader
}

void SearchTeacherInfoController::initialize()
{
    refreshSearchTeacherInfo();
}

void SearchTeacherInfoController::refreshSearchTeacherInfo()
{
    QVariantList newTeacherInfoList;
    CUtils::updateTeacherInfoList(newTeacherInfoList, mTeacherInfos);

    if (mTeacherInfoList != newTeacherInfoList)
    {
        mTeacherInfoList = std::move(newTeacherInfoList);
        emit teacherInfoListChanged();
    }
}











































