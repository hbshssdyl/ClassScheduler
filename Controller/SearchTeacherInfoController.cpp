#pragma once

#include <QCoreApplication>
#include "SearchTeacherInfoController.h"
#include "Utils/ControllerUtils.h"


#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"

#include "xlsxworkbook.h"
using namespace QXlsx;

using namespace ClassScheduler;

SearchTeacherInfoController::SearchTeacherInfoController(QObject* parent)
{
    Document doc("test.xlsx");
    doc.selectSheet("Sheet2"); // select a sheet. current sheet is 'added sheet'.
    int row = 1; int col = 1;
    QString var = doc.read( row, col ).toString();
    cout << var.toStdString() << endl;
}

void SearchTeacherInfoController::initialize()
{
    refreshSearchTeacherInfo();
}

void SearchTeacherInfoController::refreshSearchTeacherInfo()
{
    QVariantList newTeacherInfoList;
    CUtils::updateTeacherInfoList(newTeacherInfoList);

    if (mTeacherInfoList != newTeacherInfoList)
    {
        mTeacherInfoList = std::move(newTeacherInfoList);
        emit teacherInfoListChanged();
    }
    cout<<"mytest7"<<endl;
}











































