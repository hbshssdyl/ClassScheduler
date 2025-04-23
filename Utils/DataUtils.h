#pragma once

#include <String>
#include <vector>

using namespace std;

namespace ClassScheduler
{
    struct teacherInfo
    {
        std::string teacherName;
        std::string teacherNickName;

        //teacherCourseList
        std::string date;
        std::string studentName;
        std::string weekend;
        std::string school;
        std::string studentPhoneNubmer;
        std::string grade;
        std::string suject;
        std::string time;
        std::string type;
        std::string courseTime;
        std::string studentFee;
        std::string teacherFee;
    };

    static vector validExcelHeader{"日期", "姓名", "学校", "电话", "年级", "学科", "时间", "老师", "网课or面授", "老师姓名", "金额/小时", "老师工资"};
} // namespace ClassScheduler
