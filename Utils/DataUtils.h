#pragma once

#include <String>
#include <vector>

using namespace std;

namespace ClassScheduler
{
    static std::string nullString = "无该信息";
    static vector validExcelHeader{"日期", "星期", "姓名", "学校", "电话", "年级", "学科", "时间", "老师", "网课or面授", "课时", "金额/小时", "课酬总计", "老师姓名", "老师工资", "已收金额", "付费方式", "收费日期"};
    struct teacherInfo
    {
        std::string teacherName;
        std::string teacherNickName;

        //teacherCourseList
        std::string date;
        std::string weekend;
        std::string studentName;

        std::string school;
        std::string studentPhoneNubmer;
        std::string grade;
        std::string suject;
        std::string time;
        std::string learningType;
        std::string courseTime;
        std::string studentFee;
        std::string studentTotalFee;
        std::string teacherFee;
        std::string gotMoney;
        std::string payType;
        std::string payDate;

        bool isValidInfo()
        {
            int cnt = 0;
            if(studentName == nullString) return false;
            if(nullString != teacherName) cnt++;
            if(nullString != teacherNickName) cnt++;
            if(nullString != date) cnt++;
            if(nullString != weekend) cnt++;
            if(nullString != studentName) cnt++;
            if(nullString != school) cnt++;
            if(nullString != studentPhoneNubmer) cnt++;
            if(nullString != grade) cnt++;
            if(nullString != suject) cnt++;
            if(nullString != time) cnt++;
            if(nullString != learningType) cnt++;
            if(nullString != courseTime) cnt++;
            if(nullString != studentFee) cnt++;
            if(nullString != studentTotalFee) cnt++;
            if(nullString != teacherFee) cnt++;
            if(nullString != gotMoney) cnt++;
            if(nullString != payType) cnt++;
            if(nullString != payDate) cnt++;
            if(cnt < 5) return false;
            return true;
        }
    };


} // namespace ClassScheduler
