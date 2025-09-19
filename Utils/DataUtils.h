#pragma once

#include <String>
#include <vector>
#include <QString>
#include <QTime>
#include <QtSql>
#include <map>
#include <QVariant>
#include <algorithm>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Managers/Logger.h"

namespace ClassScheduler
{
    static const QString CLASS_INFOS_TABLE_NAME = "classInfos";
    static const QString TEACHER_INFOS_TABLE_NAME = "teacherInfos";
    static const QString STUDENT_INFOS_TABLE_NAME = "studentInfos";
    static const QString APP_TOGGLE_INFOS_TABLE_NAME = "toggleInfos";
    static const std::string ADMIN_USER_NAME = "Dylandu";

    static const QString DATABASE_NAME = "InfoDatabase.db";
    static const QString DATABASE_PATH_DIR = QDir::currentPath();
    static const QString DATABASE_FULL_PATH = DATABASE_PATH_DIR + "/" + DATABASE_NAME;

    static QString nullString = "无该信息";
    static std::vector validExcelClassHeader{"日期", "星期", "姓名", "学校", "电话", "年级", "学科", "时间", "老师", "网课or面授", "课时", "金额/小时", "课酬总计", "老师姓名", "老师工资", "已收金额", "付费方式", "收费日期"};
    static std::vector validTeacherHeader{"老师姓名", "使用过的昵称", "教过的科目及学生", "科目及工资（每小时）", "科目及年级"};
    static std::vector validStudentHeader{"学生姓名", "就读学校", "手机号", "教过该生的老师", "科目及费用（每小时）"};
    static std::vector allTableNameForDB{"classInfos", "teacherInfos", "studentInfos"};

    enum class OperateMode
    {
        None,
        LoginView, //once view
        FileView,  //once view
        WelcomePage,
        SearchClassInfo,
        SearchTeacherInfo,
        SearchStudentInfo,
        ScheduleClass,
        TaskAssistantView,
        TaskManagerView,
        TeacherEvaluation,
        FeedbackApp,
        FeedbackCompany,
        AccountView
    };
    using OperateModes = std::vector<OperateMode>;

    enum class ResultStatus {
        //Register
        RegisterSuccess,
        RegisterPending,
        UserExist,
        EmailExist,
        EmailInvalid,

        //User
        GetAllUsersSuccess,
        GetAllUsersFailed,
        AddUserSuccess,
        AddUserFailed,
        ApproveUserSuccess,
        ApproveUserFailed,
        RejectUserSuccess,
        RejectUserFailed,
        BlacklistUserSuccess,
        BlacklistUserFailed,
        DeleteUserSuccess,
        DeleteUserFailed,

        //Login
        LoginSuccess,
        UserOrEmailNotFound,
        PasswordIncorrect,
        AccountPending,
        AccountRejected,
        AccountBlacklisted,

        //Upload DB file
        DatabaseUploadSuccess,
        DatabaseUploadFailed,

        //Download DB file
        CreateDatabaseFileFailed,
        DatabaseFileDownloadSucess,
        DatabaseFileDownloadFailed,

        //OneToOne task
        GetOneToOneTaskSuccess,
        GetOneToOneTasksSuccess,
        AddOneToOneTaskSuccess,
        UpdateOneToOneTaskSuccess,

        //Feedback
        CreateFeedbackSuccess,
        CreateFeedbackFailed,
        UpdateFeedbackStatusSuccess,
        UpdateFeedbackStatusFailed,
        DeleteFeedbackSuccess,
        DeleteFeedbackFailed,
        GetAllFeedbacksSuccess,
        GetAllFeedbacksFailed,
        LikeFeedbackSuccess,
        LikeFeedbackFailed,

        //Other
        CurlNotOK,
        UnknownError
    };

    enum class FeedbackType {
        AppIssue,     // APP问题
        CompanyIssue  // 公司问题
    };

    enum class FeedbackUserType {
        RealName,     // 实名用户
        Anonymous     // 匿名用户
    };

    enum class FeedbackStatus {
        Submitted,    // 已提交
        Approved,     // 已通过
        Rejected      // 已拒绝
    };

    // --- FeedbackType ---
    inline FeedbackType toFeedbackType(const std::string& str) {
        if (str == "APP问题") return FeedbackType::AppIssue;
        if (str == "公司问题") return FeedbackType::CompanyIssue;
        throw std::invalid_argument("Unknown FeedbackType: " + str);
    }

    inline std::string to_string(FeedbackType v) {
        switch (v) {
        case FeedbackType::AppIssue:     return "APP问题";
        case FeedbackType::CompanyIssue: return "公司问题";
        }
        return "APP问题";
    }

    // --- FeedbackUserType ---
    inline FeedbackUserType toFeedbackUserType(const std::string& str) {
        if (str == "实名用户") return FeedbackUserType::RealName;
        if (str == "匿名用户") return FeedbackUserType::Anonymous;
        throw std::invalid_argument("Unknown FeedbackUserType: " + str);
    }

    inline std::string to_string(FeedbackUserType v) {
        switch (v) {
        case FeedbackUserType::RealName: return "实名用户";
        case FeedbackUserType::Anonymous:return "匿名用户";
        }
        return "匿名用户";
    }

    // --- FeedbackStatus ---
    inline FeedbackStatus toFeedbackStatus(const std::string& str) {
        if (str == "已提交") return FeedbackStatus::Submitted;
        if (str == "已通过") return FeedbackStatus::Approved;
        if (str == "已拒绝") return FeedbackStatus::Rejected;
        throw std::invalid_argument("Unknown FeedbackStatus: " + str);
    }

    inline std::string to_string(FeedbackStatus v) {
        switch (v) {
        case FeedbackStatus::Submitted: return "已提交";
        case FeedbackStatus::Approved:  return "已通过";
        case FeedbackStatus::Rejected:  return "已拒绝";
        }
        return "已提交";
    }

    struct FeedbackCreateReq {
        FeedbackType type;                  // 反馈类型
        FeedbackUserType userType;          // 用户类型（实名/匿名）
        std::string message;                // 反馈信息
        std::optional<std::string> feedbackUsername; // 展示用昵称（可选）
        std::optional<std::string> realUsername;     // 真实用户名（实名时可带）
        std::optional<std::string> realEmail;        // 真实邮箱（实名时可带）
        std::optional<std::string> dateAndTime;        // 真实邮箱（实名时可带）
    };

    struct FeedbackUpdateStatusReq {
        int id;
        FeedbackStatus newStatus;
    };

    struct FeedbackInfo {
        int id;
        FeedbackType type;                  // 反馈类型
        FeedbackUserType userType;          // 用户类型（实名/匿名）
        int likeCount;
        FeedbackStatus feedbackStatus;                // 反馈状态
        std::string message;                // 反馈信息
        std::string feedbackUsername; // 展示用昵称（可选）
        std::string realUsername;     // 真实用户名（实名时可带）
        std::string realEmail;        // 真实邮箱（实名时可带）
        std::string dateAndTime;
    };
    using FeedbackInfos = std::vector<FeedbackInfo>;

    enum class AccountStatus {
        REGISTERED,
        APPROVED,
        REJECTED,
        BLACKLISTED
    };

    inline const std::unordered_map<AccountStatus, std::string> AccountStatusNames = {
        { AccountStatus::REGISTERED,  "已注册" },
        { AccountStatus::APPROVED,    "已通过" },
        { AccountStatus::REJECTED,    "已拒绝" },
        { AccountStatus::BLACKLISTED, "黑名单" }
    };

    inline std::string toAccountStatus(AccountStatus status) {
        return AccountStatusNames.at(status);
    }

    enum class UserRole {
        None,
        Staff,
        SeniorStaff,
        Manager,
        Boss,
        OneToOneAssistant,
        OneToOneManager,
        OneToMultiAssistant,
        OneToMultiManager,
        SuperAdmin
    };

    inline std::string to_string(UserRole v) {
        switch(v) {
        case UserRole::None: return "无";
        case UserRole::Staff: return "员工";
        case UserRole::SeniorStaff: return "高级员工";
        case UserRole::Manager: return "经理";
        case UserRole::Boss: return "老板";
        case UserRole::OneToOneAssistant: return "一对一助理";
        case UserRole::OneToOneManager: return "一对一经理";
        case UserRole::OneToMultiAssistant: return "一对多助理";
        case UserRole::OneToMultiManager: return "一对多经理";
        case UserRole::SuperAdmin: return "超级管理员";
        default: return "未知角色";
        }
    }

    inline UserRole toUserRole(const std::string& str) {
        if (str == "无") return UserRole::None;
        if (str == "员工") return UserRole::Staff;
        if (str == "高级员工") return UserRole::SeniorStaff;
        if (str == "经理") return UserRole::Manager;
        if (str == "老板") return UserRole::Boss;
        if (str == "一对一助理") return UserRole::OneToOneAssistant;
        if (str == "一对一经理") return UserRole::OneToOneManager;
        if (str == "一对多助理") return UserRole::OneToMultiAssistant;
        if (str == "一对多经理") return UserRole::OneToMultiManager;
        if (str == "超级管理员") return UserRole::SuperAdmin;
        throw std::invalid_argument("Unknown UserRole: " + str);
    }

    struct Task {
        int id;
        bool isOverdue;

        std::string title;
        std::string category;
        std::string description;
        std::string publish;
        std::string due;
        std::string rating;
        std::string finishStatus;//未完成，已完成，已过期
        std::string comment;
        std::string reviewString;
        std::string resultRating;
        std::string reviewStatus;//已审核，未审核

        Task() = default;

        // 构造函数，便于初始化
        Task(const std::string& t, const std::string& c, const std::string& d,
             const std::string& p, const std::string& du, const std::string& r,
             const std::string& fs, const std::string& cm, const std::string& rs,
             const std::string& rr, const std::string& rv)
            : title(t), category(c), description(d), publish(p), due(du), rating(r),
            finishStatus(fs), comment(cm), reviewString(rs), resultRating(rr), reviewStatus(rv) {}

        void print() const {
            LOG_INFO("Task Details:\n");
            LOG_INFO("ID: " + std::to_string(id));
            LOG_INFO("Title: " +title);
            LOG_INFO("Category: " +category);
            LOG_INFO("Description: " +description);
            LOG_INFO("Publish Date: " +publish);
            LOG_INFO("Due Date: " +due);
            LOG_INFO("Rating: " +rating);
            LOG_INFO("Status: " +finishStatus);
            LOG_INFO("Comment: " +comment);
            LOG_INFO("Review: " +reviewString);
            LOG_INFO("Result Rating: " +resultRating);
            LOG_INFO("Review Status: " +reviewStatus);
            std::string isOverDue = (isOverdue ? "Yes" : "No");
            LOG_INFO("Is Overdue: " + isOverDue);
            LOG_INFO("-----------------------------\n");
        }
    };
    using Tasks = std::vector<Task>;

    struct TaskTemplate {
        std::string title;
        std::string category;
        std::string description;
        std::string rating;
    };


    struct UserInfo{
        int id;
        std::string name;
        std::string password;
        std::string email;
        std::string roleStr;
        std::string accountStatus;

        UserRole role;
        OperateModes mods;
    };
    using UserInfos = std::vector<UserInfo>;

    struct Setting {
        QString key;
        bool value;
    };
    static QVector<Setting> initialAppSettings = {
        { "showStudentAllClass",      true  },
        { "showStudentSujectClass",   false },
        { "showStudentSujectScore",   false },
        { "showTeacherAllStudent",    true  },
        { "showTeacherSujectStudent", false }
    };

    struct MonthCountInfo {
        QString yearMonth;
        std::vector<QString> keys;
        int keyCount;

        MonthCountInfo()
        {
        }

        MonthCountInfo(QString yMonth, int count)
        {
            yearMonth = yMonth;
            keyCount = count;
        }

        MonthCountInfo(QString yMonth, QString key)
        {
            yearMonth = yMonth;
            keys.emplace_back(key);
            keyCount = 1;
        }

        void saveKey(QString name)
        {
            bool save = true;
            for(auto& val : keys)
            {
                if(name == val)
                {
                    save = false;
                    break;
                }
            }
            if(save)
            {
                keys.emplace_back(name);
                keyCount++;
            }
        }
    };
    using MonthCountInfos = std::vector<MonthCountInfo>;

    struct SujectCountInfo {
        QString suject;
        MonthCountInfos monthCountInfos;

        SujectCountInfo(QString sujectStr, QString yMonth, QString keyName)
        {
            suject = sujectStr;
            MonthCountInfo monthCountInfo(yMonth, keyName);
            monthCountInfos.emplace_back(monthCountInfo);
        }

        void sortMonthStudentInfosByYearMonth() {
            std::sort(monthCountInfos.begin(), monthCountInfos.end(), [](const MonthCountInfo& a, const MonthCountInfo& b) {
                return a.yearMonth < b.yearMonth;
            });
        }

        void fillMissingMonths(const QString& minYearMonth, const QString& maxYearMonth) {
            MonthCountInfos result;

            // 检查日期格式是否正确（yyyy-MM）
            if (minYearMonth.length() != 7 || maxYearMonth.length() != 7 ||
                minYearMonth[4] != '-' || maxYearMonth[4] != '-') {
                LOG_INFO("Invalid date format! Expected yyyy-MM." +minYearMonth.toStdString() +", " +maxYearMonth.toStdString());
                return;
            }

            QDate minDate = QDate::fromString(minYearMonth + "-01", "yyyy-MM-dd");
            QDate maxDate = QDate::fromString(maxYearMonth + "-01", "yyyy-MM-dd");

            // 检查日期是否有效
            if (!minDate.isValid() || !maxDate.isValid()) {
                //LOG_INFO("Invalid date range! minDate:" +minDate +"maxDate:" +maxDate);
                return;
            }

            if (minDate > maxDate) {
                return; // 如果 min > max，直接返回
            }

            // 将现有数据存入 QMap 以便快速查找
            QMap<QString, MonthCountInfo> existingData;
            for (const auto& info : monthCountInfos) {
                existingData[info.yearMonth] = info;
            }

            // 遍历从 minDate 到 maxDate 的每个月
            QDate currentDate = minDate;
            while (currentDate <= maxDate) {
                QString currentYearMonth = currentDate.toString("yyyy-MM");

                // 如果该月份已有数据，则直接使用；否则补全为 0
                if (existingData.contains(currentYearMonth)) {
                    result.push_back(existingData[currentYearMonth]);
                } else {
                    result.push_back(MonthCountInfo{currentYearMonth, 0});
                }

                QDate nextDate = currentDate.addMonths(1);
                if (!nextDate.isValid() || nextDate <= currentDate) {
                    //LOG_INFO("Failed to move to next month! currentDate:" +currentDate);
                    break;
                }
                currentDate = nextDate;
            }

            monthCountInfos = result;
        }

        QVariantMap toMapStyle()
        {
            QStringList monthCountList;
            for(auto& info : monthCountInfos)
            {
                monthCountList.append(QString::number(info.keyCount));
            }
            QStringList yearMonthList;
            for(auto& info : monthCountInfos)
            {
                yearMonthList.append(info.yearMonth);
            }
            return QVariantMap{ {"suject", suject},
                               {"monthCountList", monthCountList},
                               {"yearMonthList", yearMonthList} };
        }
    };
    using SujectCountInfos = std::vector<SujectCountInfo>;

    struct DataBasicInfo {
        //学生课程（教师所教学生）数量曲线图基础信息
        int maxKeyCount;//所有课程或学生数量中各月份中的最大值
        QString minYearMonth;//横轴的最小月份
        QString maxYearMonth;//横轴的最大月份
        bool showTotal;//显示总数的曲线图
        bool showEvery;//显示各科目的key数量曲线图

        DataBasicInfo()
        {
            maxKeyCount = 0;
            showTotal = false;
            showEvery = false;
        }

        void refreshData(const QString& newYearMonth, int keyCount) {
            if(!newYearMonth.isEmpty())
            {
                if (minYearMonth.isEmpty() || newYearMonth < minYearMonth)
                {
                    minYearMonth = newYearMonth;
                }
                if (maxYearMonth.isEmpty() || newYearMonth > maxYearMonth)
                {
                    maxYearMonth = newYearMonth;
                }
            }

            if(keyCount > maxKeyCount)
            {
                maxKeyCount = keyCount;
            }
        }

        void clear()
        {
            maxKeyCount = 0;
            minYearMonth = "";
            maxYearMonth = "";
        }
    };

    struct ClassInfo
    {
        QString date;
        QString weekend;
        QString studentName;
        QString school;
        QString studentPhoneNubmer;
        QString grade;

        QString suject;
        QString time;
        QString teacherNickName;
        QString learningType;
        QString courseTime;
        QString studentFee;

        QString studentTotalFee;
        QString teacherName;
        QString teacherFee;
        QString gotMoney;
        QString payType;
        QString payDate;

        QStringList toInfosList(QString id)
        {
            QStringList list;
            if(!id.isEmpty())
            {
                list.append(id);
            }
            list.append(date);list.append(weekend);list.append(studentName);list.append(school);list.append(studentPhoneNubmer);list.append(grade);
            list.append(suject);list.append(time);list.append(teacherNickName);list.append(learningType);list.append(courseTime);list.append(studentFee);
            list.append(studentTotalFee);list.append(teacherName);list.append(teacherFee);list.append(gotMoney);list.append(payType);list.append(payDate);
            return list;
        }

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

        bool isContains(QString str)
        {
            if(date.contains(str, Qt::CaseInsensitive)) return true;
            if(weekend.contains(str, Qt::CaseInsensitive)) return true;
            if(studentName.contains(str, Qt::CaseInsensitive)) return true;
            if(school.contains(str, Qt::CaseInsensitive)) return true;
            if(studentPhoneNubmer.contains(str, Qt::CaseInsensitive)) return true;
            if(grade.contains(str, Qt::CaseInsensitive)) return true;

            if(suject.contains(str, Qt::CaseInsensitive)) return true;
            if(time.contains(str, Qt::CaseInsensitive)) return true;
            if(teacherNickName.contains(str, Qt::CaseInsensitive)) return true;
            if(learningType.contains(str, Qt::CaseInsensitive)) return true;
            if(courseTime.contains(str, Qt::CaseInsensitive)) return true;
            if(studentFee.contains(str, Qt::CaseInsensitive)) return true;

            if(studentTotalFee.contains(str, Qt::CaseInsensitive)) return true;
            if(teacherName.contains(str, Qt::CaseInsensitive)) return true;
            if(teacherFee.contains(str, Qt::CaseInsensitive)) return true;
            if(gotMoney.contains(str, Qt::CaseInsensitive)) return true;
            if(payType.contains(str, Qt::CaseInsensitive)) return true;
            if(payDate.contains(str, Qt::CaseInsensitive)) return true;
            return false;
        }

        bool isAllContains(QString str)
        {
            str.replace(QChar(u'，'), QChar(','));
            QStringList parts = str.split(',');
            bool result = true;
            for (const QString& part : parts) {
                if(!isContains(part))
                {
                    result = false;
                    break;
                }
            }
            return result;
        }
    };
    using ClassInfos = std::vector<ClassInfo>;

    struct TeacherInfo
    {
        QString teacherName;
        std::vector<QString> teacherNickNames;
        std::vector<QString> teacherSujectsAndStudents;
        std::vector<QString> teacherSujectsAndFees;
        std::vector<QString> teacherSujectsAndGrades;

        QString strTeacherNickNames;
        QString strTeacherSujectsAndStudents;
        QString strTeacherSujectsAndFees;
        QString strTeacherSujectsAndGrades;

        SujectCountInfos sujectStudentCounts;
        DataBasicInfo teacherStudentCountBasicInfo;

        TeacherInfo()
        {
        }

        TeacherInfo(const QString teacherName)
            : teacherName(teacherName)
        {
        }

        void addSujectCountInfo(QString suject, QString date, QString studentName)
        {
            auto yearMonth = getYearMonth(date);

            bool hasSuject = false;
            for(auto& sujectCountInfo : sujectStudentCounts)
            {
                if(suject == sujectCountInfo.suject)
                {
                    hasSuject = true;
                    bool hasYearMonth = false;
                    for(auto& monthStudentInfo : sujectCountInfo.monthCountInfos)
                    {
                        if(yearMonth == monthStudentInfo.yearMonth)
                        {
                            hasYearMonth = true;
                            monthStudentInfo.saveKey(studentName);
                            break;
                        }
                    }
                    if(!hasYearMonth)
                    {
                        MonthCountInfo monthCountInfo(yearMonth, studentName);
                        sujectCountInfo.monthCountInfos.emplace_back(monthCountInfo);
                    }
                    break;
                }
            }
            if(!hasSuject)
            {
                SujectCountInfo sujectStudentCount(suject, yearMonth, studentName);
                sujectStudentCounts.emplace_back(sujectStudentCount);
            }
        }

        QString getYearMonth(const QString& dateStr) {
            QStringList parts = dateStr.split("-");
            if (parts.size() >= 2) {
                return parts[0] + "-" + parts[1];
            }
            return "";
        }

        QStringList toInfosList(QString id)
        {
            QStringList list;
            if(!id.isEmpty())
            {
                list.append(id);
            }
            list.append(teacherName);
            list.append(strTeacherNickNames);
            list.append(strTeacherSujectsAndStudents);
            list.append(strTeacherSujectsAndFees);
            list.append(strTeacherSujectsAndGrades);
            return list;
        }

        void sortInfos()
        {
            sort(teacherNickNames.begin(), teacherNickNames.end());
            sort(teacherSujectsAndStudents.begin(), teacherSujectsAndStudents.end());
            sort(teacherSujectsAndFees.begin(), teacherSujectsAndFees.end());
            sort(teacherSujectsAndGrades.begin(), teacherSujectsAndGrades.end());
        }

        QString getString(std::vector<QString> stringList)
        {
            QString ret = "";
            bool flag = false;
            for(auto& str : stringList)
            {
                if(flag)
                {
                    ret += ", ";
                }
                ret += str;
                flag = true;
            }
            return ret;
        }

        void saveValue(QString value, std::vector<QString>& values)
        {
            bool save = true;
            for(auto& val : values)
            {
                if(value == val)
                {
                    save = false;
                    break;
                }
            }
            if(save)
            {
                values.emplace_back(value);
            }
        }

        QString formatSubjects(const std::vector<QString>& input) {
            std::map<QString, QStringList> subjectScores;
            for (const QString& item : input) {
                QStringList parts = item.split('_');
                if (parts.size() == 2) {
                    subjectScores[parts[0]].append(parts[1]);
                }
            }
            QStringList result;
            for (const auto& pair : subjectScores) {
                result << QString("%1：%2").arg(pair.first, pair.second.join("，"));
            }
            return result.join('\n');
        }

        QString getTeacherNickNames()
        {
            return getString(teacherNickNames);
        }

        QString getTeacherSujectsAndStudents()
        {
            return formatSubjects(teacherSujectsAndStudents);
        }

        QString getTeacherSujectsAndFees()
        {
            return formatSubjects(teacherSujectsAndFees);
        }

        QString getTeacherSujectsAndGrades()
        {
            return formatSubjects(teacherSujectsAndGrades);
        }

        bool isContains(QString str)
        {
            if(teacherName.contains(str, Qt::CaseSensitive)) return true;
            if(strTeacherNickNames.contains(str, Qt::CaseSensitive)) return true;
            if(strTeacherSujectsAndFees.contains(str, Qt::CaseSensitive)) return true;
            if(strTeacherSujectsAndGrades.contains(str, Qt::CaseSensitive)) return true;
            return false;
        }

        bool isAllContains(QString str)
        {
            str.replace(QChar(u'，'), QChar(','));
            QStringList parts = str.split(',');
            bool result = true;
            for (const QString& part : parts) {
                if(!isContains(part))
                {
                    result = false;
                    break;
                }
            }
            return result;
        }
    };
    using TeacherInfos = std::vector<TeacherInfo>;

    struct StudentInfo
    {
        QString studentName;
        std::vector<QString> studentSchools;
        std::vector<QString> studentPhoneNumbers;
        std::vector<QString> studentTeachers; // 张三（张三昵称）
        std::vector<QString> studentSujectsAndPays; // 物理： 100(张三)，200（李四）

        QString strStudentSchools;
        QString strStudentPhoneNumbers;
        QString strStudentTeachers;
        QString strStudentSujectsAndPays;

        SujectCountInfos sujectClassCounts;
        DataBasicInfo studentClassCountBasicInfo;

        StudentInfo()
        {
        }

        StudentInfo(const QString studentName)
            : studentName(studentName)
        {
        }

        QStringList toInfosList(QString id)
        {
            QStringList list;
            if(!id.isEmpty())
            {
                list.append(id);
            }
            list.append(studentName);
            list.append(strStudentSchools);
            list.append(strStudentPhoneNumbers);
            list.append(strStudentTeachers);
            list.append(strStudentSujectsAndPays);
            return list;
        }

        void sortInfos()
        {
        }

        QString getString(std::vector<QString> stringList)
        {
            QString ret = "";
            bool flag = false;
            for(auto& str : stringList)
            {
                if(flag)
                {
                    ret += ", ";
                }
                ret += str;
                flag = true;
            }
            return ret;
        }

        void saveValue(QString value, std::vector<QString>& values)
        {
            bool save = true;
            for(auto& val : values)
            {
                if(value == val)
                {
                    save = false;
                    break;
                }
            }
            if(save)
            {
                values.emplace_back(value);
            }
        }

        QString getFormatString(const std::vector<QString>& input) {
            std::map<QString, QStringList> subjectScores;
            for (const QString& item : input) {
                QStringList parts = item.split('_');
                if (parts.size() == 2) {
                    subjectScores[parts[0]].append(parts[1]);
                }
            }
            QStringList result;
            for (const auto& pair : subjectScores) {
                result << QString("%1：%2").arg(pair.first, pair.second.join("，"));
            }
            return result.join('\n');
        }

        QString getStudentSchools()
        {
            return getString(studentSchools);
        }

        QString getStudentPhoneNumbers()
        {
            return getString(studentPhoneNumbers);
        }

        QString getStudentTeachers()
        {
            return getFormatString(studentTeachers);
        }

        QString getStudentSujectsAndPays()
        {
            return getFormatString(studentSujectsAndPays);
        }

        bool isContains(QString str)
        {
            if(studentName.contains(str, Qt::CaseSensitive)) return true;
            if(strStudentSchools.contains(str, Qt::CaseSensitive)) return true;
            if(strStudentPhoneNumbers.contains(str, Qt::CaseSensitive)) return true;
            if(strStudentTeachers.contains(str, Qt::CaseSensitive)) return true;
            if(strStudentSujectsAndPays.contains(str, Qt::CaseSensitive)) return true;
            return false;
        }

        bool isAllContains(QString str)
        {
            str.replace(QChar(u'，'), QChar(','));
            QStringList parts = str.split(',');
            bool result = true;
            for (const QString& part : parts) {
                if(!isContains(part))
                {
                    result = false;
                    break;
                }
            }
            return result;
        }

        void addSujectCountInfo(QString suject, QString date, QString grade)
        {
            auto className = grade;
            auto yearMonth = getYearMonth(date);

            bool hasSuject = false;
            for(auto& sujectCountInfo : sujectClassCounts)
            {
                if(suject == sujectCountInfo.suject)
                {
                    hasSuject = true;
                    bool hasYearMonth = false;
                    for(auto& monthStudentInfo : sujectCountInfo.monthCountInfos)
                    {
                        if(yearMonth == monthStudentInfo.yearMonth)
                        {
                            hasYearMonth = true;
                            monthStudentInfo.saveKey(className);
                            break;
                        }
                    }
                    if(!hasYearMonth)
                    {
                        MonthCountInfo monthCountInfo(yearMonth, className);
                        sujectCountInfo.monthCountInfos.emplace_back(monthCountInfo);
                    }
                    break;
                }
            }
            if(!hasSuject)
            {
                SujectCountInfo sujectClassCount(suject, yearMonth, className);
                sujectClassCounts.emplace_back(sujectClassCount);
            }
        }

        QString getYearMonth(const QString& dateStr) {
            QStringList parts = dateStr.split("-");
            if (parts.size() >= 2) {
                return parts[0] + "-" + parts[1];
            }
            return "";
        }
    };
    using StudentInfos = std::vector<StudentInfo>;

    struct ScheduleClassInputInfo
    {
        QString suject;
        QString grade;
        QString week;
        QString startTime;
        QString endTime;
        QString timeLength;//单位小时
        QString timeRange;

        ScheduleClassInputInfo()
        {
        }

        ScheduleClassInputInfo(QVariantList requiredInfosList)
        {
            if(requiredInfosList.size() < 5)
            {
                return;
            }
            suject = requiredInfosList[0].toString();
            grade = requiredInfosList[1].toString();
            week = requiredInfosList[2].toString();
            startTime = requiredInfosList[3].toString();
            endTime = requiredInfosList[4].toString();
            timeRange = startTime + "-" + endTime;
            timeLength = calculateHoursInRange(timeRange);
        }

        ScheduleClassInputInfo(QString suject, QString grade, QString week, QString startTime, QString endTime)
            : suject(suject)
            , grade(grade)
            , week(week)
            , startTime(startTime)
            , endTime(endTime)
        {
            timeRange = startTime + "-" + endTime;
        }

        QString calculateHoursInRange(const QString& requiredTimeRange) {
            QStringList timeParts = requiredTimeRange.split("-");
            if (timeParts.size() != 2) {
                return "Invalid input";
            }

            QTime startTime = QTime::fromString(timeParts[0], "hh:mm");
            QTime endTime = QTime::fromString(timeParts[1], "hh:mm");

            if (!startTime.isValid() || !endTime.isValid()) {
                return "Invalid input";
            }

            double durationInHours = startTime.secsTo(endTime) / 3600;

            return QString::number(durationInHours);
        }
    };

    struct TimeRange {
        int startHour;
        int startMinute;
        int endHour;
        int endMinute;
    };

    struct ScheduleClassResultInfo
    {
        QString teacherName;
        QString week;
        std::vector<QString> teacherNickNames;
        std::vector<QString> teacherGradeFees;
        std::vector<QString> teacherFreeTime;
        std::vector<QString> teacherValidFreeTime;
        std::vector<QString> teacherWorkTime;
        std::vector<QString> teacherValidWorkTime;
        //vector<QString> teacherWorkGrade;

        QString strTeacherNickNames;
        QString strTeacherGradeFees;
        QString strTeacherFreeTime;
        QString strTeacherValidFreeTime;
        QString strTeacherWorkTime;
        QString strTeacherValidWorkTime;
        //QString strTeacherWorkGrade;

        ScheduleClassResultInfo()
        {
        }

        void sortInfos()
        {
            sort(teacherNickNames.begin(), teacherNickNames.end());
            sort(teacherGradeFees.begin(), teacherGradeFees.end());
            sort(teacherFreeTime.begin(), teacherFreeTime.end());
            sort(teacherValidFreeTime.begin(), teacherValidFreeTime.end());
            sort(teacherWorkTime.begin(), teacherWorkTime.end());
            sort(teacherValidWorkTime.begin(), teacherValidWorkTime.end());
            //sort(teacherWorkGrade.begin(), teacherWorkGrade.end());
        }

        QString getString(std::vector<QString> stringList)
        {
            QString ret = "";
            bool flag = false;
            for(auto& str : stringList)
            {
                if(flag)
                {
                    ret += ", ";
                }
                ret += str;
                flag = true;
            }
            return ret;
        }

        void generateInfoString()
        {
            strTeacherNickNames = getString(teacherNickNames);
            strTeacherGradeFees = getString(teacherGradeFees);
            strTeacherFreeTime = getString(teacherFreeTime);
            strTeacherValidFreeTime = getString(teacherValidFreeTime);
            strTeacherWorkTime = getString(teacherWorkTime);
            strTeacherValidWorkTime = getString(teacherValidWorkTime);
            //strTeacherWorkGrade = getString(teacherWorkGrade);
            if(strTeacherValidWorkTime.isEmpty())
            {
                strTeacherValidWorkTime = week + "无课";
            }
        }

        void generateValidWorkTime()
        {
            std::unordered_map<QString, int> timeCount;

            for (const auto& time : teacherWorkTime) {
                timeCount[time]++;
            }

            for (const auto& [time, count] : timeCount) {
                if (count >= 2) {
                    teacherValidWorkTime.emplace_back(time);
                }
            }
        }

        void generateValidFreeTime(QString timeLengthStr)
        {
            bool ok;
            double timeLength = timeLengthStr.toDouble(&ok); // Convert QString to double
            if (!ok) {
                return;
            }

            for (const auto& range : teacherFreeTime) {
                QStringList times = range.split('-');
                if (times.size() == 2) {
                    QString startTime = times[0];
                    QString endTime = times[1];

                    QTime start = QTime::fromString(startTime, "hh:mm");
                    QTime end = QTime::fromString(endTime, "hh:mm");

                    if (start.isValid() && end.isValid()) {
                        double duration = start.secsTo(end) / 3600.0; // Convert seconds to hours
                        if (duration >= timeLength) {
                            teacherValidFreeTime.emplace_back(range);
                        }
                    }
                }
            }
        }

        bool isTimeRangeContained(const QString& requiredTimeRange, const std::vector<QString>& freeTimeRange) {
            auto parseTime = [](const QString& time) {
                int hours = time.mid(0, 2).toInt();
                int minutes = time.mid(3, 2).toInt();
                return hours * 60 + minutes; // Convert time to minutes
            };

            int requiredStart = parseTime(requiredTimeRange.split('-')[0]);
            int requiredEnd = parseTime(requiredTimeRange.split('-')[1]);

            for (const auto& range : freeTimeRange) {
                int freeStart = parseTime(range.split('-')[0]);
                int freeEnd = parseTime(range.split('-')[1]);

                if (freeStart <= requiredStart && freeEnd >= requiredEnd) {
                    return true; // Found a free time range that contains the required time range
                }
            }

            return false; // No free time range contains the required time range
        }

        bool isValid(const QString& requiredTimeRange)
        {
            return isTimeRangeContained(requiredTimeRange, teacherFreeTime);
        }

        void generateTeacherFreeTime()
        {
            // 定义一天的起始和结束时间
            QTime startOfDay = QTime::fromString("08:00", "HH:mm");
            QTime endOfDay = QTime::fromString("23:00", "HH:mm");

            // 将工作时间段转换为 QTime 对象的 pair
            std::vector<std::pair<QTime, QTime>> workTimes;
            for (const auto& range : teacherValidWorkTime) {
                QStringList times = range.split("-");
                if (times.size() == 2) {
                    QTime start = QTime::fromString(times[0], "HH:mm");
                    QTime end = QTime::fromString(times[1], "HH:mm");
                    if (start.isValid() && end.isValid()) {
                        workTimes.emplace_back(start, end);
                    }
                }
            }

            // 按起始时间排序工作时间段
            std::sort(workTimes.begin(), workTimes.end(), [](const auto& a, const auto& b) {
                return a.first < b.first;
            });

            // 计算剩余时间段
            QTime currentStart = startOfDay;

            for (const auto& workTime : workTimes) {
                if (currentStart < workTime.first) {
                    // 如果当前时间段的开始时间小于工作时间段的开始时间，说明有空闲时间
                    teacherFreeTime.push_back(
                        QString("%1-%2").arg(currentStart.toString("HH:mm")).arg(workTime.first.toString("HH:mm"))
                        );
                }
                // 更新当前时间段的开始时间
                if (currentStart < workTime.second) {
                    currentStart = workTime.second;
                }
            }

            // 检查最后一个时间段是否有剩余
            if (currentStart < endOfDay) {
                teacherFreeTime.push_back(
                    QString("%1-%2").arg(currentStart.toString("HH:mm")).arg(endOfDay.toString("HH:mm"))
                    );
            }
        }

        void saveValue(QString value, std::vector<QString>& values, bool removeDuplicate = true)
        {
            bool save = true;
            for(auto& val : values)
            {
                if(removeDuplicate && value == val)
                {
                    save = false;
                    break;
                }
            }
            if(save)
            {
                values.emplace_back(value);
            }
        }
    };

    struct ResponseResult {
        ResultStatus status;
        std::string statusStr; // 具体描述
        std::string rawResponse; // 原始响应字符串（用于调试）

        //login
        UserRole role;
        std::string username;

        //User
        UserInfos userInfos;

        //OneToOneTasks
        Tasks oneToOneTasks;
        int taskId;

        //FeedbackInfos
        FeedbackInfos feedbackInfos;

        ResponseResult()
        {
            rawResponse = "ResponseResult: init rawResponse";
            status = ResultStatus::UnknownError;
            statusStr = toString(ResultStatus::UnknownError);
        }

        void updateToCurlError()
        {
            rawResponse = "Fail to init curl";
            status = ResultStatus::CurlNotOK;
            statusStr = toString(ResultStatus::CurlNotOK);
        }

        void sortFeedbackByDateTime(FeedbackInfos& feedbacks) {
            std::sort(feedbacks.begin(), feedbacks.end(), [](const FeedbackInfo& a, const FeedbackInfo& b) {
                return a.dateAndTime > b.dateAndTime;
            });
        }

        void refreshResult(std::string response)
        {
            //LOG_INFO(response);
            rawResponse = response;

            // Login cases
            if(response.find(toString(ResultStatus::LoginSuccess), Qt::CaseSensitive) != std::string::npos)
            {
                status = ResultStatus::LoginSuccess;
                statusStr = toString(ResultStatus::LoginSuccess);

                nlohmann::json j = nlohmann::json::parse(response);
                std::map<std::string, std::string> result = j.get<std::map<std::string, std::string>>();

                for (const auto& [key, value] : result) {
                    if(key == "role"){
                        role = toUserRole(value);
                    }
                    if(key == "username"){
                        username = value;
                    }
                }
                return;
            }

            // Feedback case
            if (response.find(toString(ResultStatus::GetAllFeedbacksSuccess), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::GetAllFeedbacksSuccess;
                statusStr = toString(ResultStatus::GetAllFeedbacksSuccess);

                nlohmann::json feedbackArray = nlohmann::json::parse(response);

                for (const auto& item : feedbackArray["feedbacks"]) {
                    FeedbackInfo feedback;

                    feedback.id = item.at("id").get<int>();
                    feedback.type = toFeedbackType(item.at("feedbackType").get<std::string>());
                    feedback.userType = toFeedbackUserType(item.at("userType").get<std::string>());
                    feedback.feedbackStatus = toFeedbackStatus(item.at("feedbackStatus").get<std::string>());
                    feedback.likeCount = item.at("likeCount").get<int>();

                    feedback.message = item.at("feedbackMessage").get<std::string>();
                    feedback.feedbackUsername = item.value("feedbackUsername", "");
                    feedback.realUsername = item.value("realUsername", "");
                    feedback.realEmail = item.value("realEmail", "");
                    feedback.dateAndTime = item.value("dateAndTime", "");

                    feedbackInfos.push_back(feedback);
                }

                sortFeedbackByDateTime(feedbackInfos);

                return;
            }

            if (response.find(toString(ResultStatus::GetAllFeedbacksFailed), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::GetAllFeedbacksFailed;
                statusStr = toString(ResultStatus::GetAllFeedbacksFailed);
                return;
            }

            if (response.find(toString(ResultStatus::CreateFeedbackSuccess), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::CreateFeedbackSuccess;
                statusStr = toString(ResultStatus::CreateFeedbackSuccess);
                return;
            }

            if (response.find(toString(ResultStatus::CreateFeedbackFailed), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::CreateFeedbackFailed;
                statusStr = toString(ResultStatus::CreateFeedbackFailed);
                return;
            }

            if (response.find(toString(ResultStatus::UpdateFeedbackStatusSuccess), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::UpdateFeedbackStatusSuccess;
                statusStr = toString(ResultStatus::UpdateFeedbackStatusSuccess);
                return;
            }

            if (response.find(toString(ResultStatus::UpdateFeedbackStatusFailed), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::UpdateFeedbackStatusFailed;
                statusStr = toString(ResultStatus::UpdateFeedbackStatusFailed);
                return;
            }

            if (response.find(toString(ResultStatus::DeleteFeedbackSuccess), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::DeleteFeedbackSuccess;
                statusStr = toString(ResultStatus::DeleteFeedbackSuccess);
                return;
            }

            if (response.find(toString(ResultStatus::DeleteFeedbackFailed), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::DeleteFeedbackFailed;
                statusStr = toString(ResultStatus::DeleteFeedbackFailed);
                return;
            }

            if (response.find(toString(ResultStatus::LikeFeedbackSuccess), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::LikeFeedbackSuccess;
                statusStr = toString(ResultStatus::LikeFeedbackSuccess);
                return;
            }

            if (response.find(toString(ResultStatus::LikeFeedbackFailed), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::LikeFeedbackFailed;
                statusStr = toString(ResultStatus::LikeFeedbackFailed);
                return;
            }

            // User case
            if (response.find(toString(ResultStatus::GetAllUsersSuccess), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::GetAllUsersSuccess;
                statusStr = toString(ResultStatus::GetAllUsersSuccess);

                nlohmann::json taskArray = nlohmann::json::parse(response);
                // 确保 tasks 是数组
                for (const auto& item : taskArray["users"]) {
                    UserInfo userInfo;
                    userInfo.id = item.at("id").get<int>();
                    userInfo.name = item.at("username").get<std::string>();
                    userInfo.email = item.at("email").get<std::string>();
                    userInfo.password = item.at("password").get<std::string>();
                    userInfo.roleStr = item.at("role").get<std::string>();
                    userInfo.accountStatus = item.at("account_status").get<std::string>();
                    userInfo.role = toUserRole(userInfo.roleStr);
                    userInfos.push_back(userInfo);
                }

                return;
            }

            if (response.find(toString(ResultStatus::GetAllUsersFailed), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::GetAllUsersFailed;
                statusStr = toString(ResultStatus::GetAllUsersFailed);
                return;
            }

            if (response.find(toString(ResultStatus::AddUserSuccess), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::AddUserSuccess;
                statusStr = toString(ResultStatus::AddUserSuccess);
                return;
            }

            if (response.find(toString(ResultStatus::AddUserFailed), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::AddUserFailed;
                statusStr = toString(ResultStatus::AddUserFailed);
                return;
            }

            if (response.find(toString(ResultStatus::ApproveUserSuccess), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::ApproveUserSuccess;
                statusStr = toString(ResultStatus::ApproveUserSuccess);
                return;
            }

            if (response.find(toString(ResultStatus::ApproveUserFailed), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::ApproveUserFailed;
                statusStr = toString(ResultStatus::ApproveUserFailed);
                return;
            }

            if (response.find(toString(ResultStatus::RejectUserSuccess), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::RejectUserSuccess;
                statusStr = toString(ResultStatus::RejectUserSuccess);
                return;
            }

            if (response.find(toString(ResultStatus::RejectUserFailed), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::RejectUserFailed;
                statusStr = toString(ResultStatus::RejectUserFailed);
                return;
            }

            if (response.find(toString(ResultStatus::BlacklistUserSuccess), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::BlacklistUserSuccess;
                statusStr = toString(ResultStatus::BlacklistUserSuccess);
                return;
            }

            if (response.find(toString(ResultStatus::BlacklistUserFailed), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::BlacklistUserFailed;
                statusStr = toString(ResultStatus::BlacklistUserFailed);
                return;
            }

            if (response.find(toString(ResultStatus::DeleteUserSuccess), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::DeleteUserSuccess;
                statusStr = toString(ResultStatus::DeleteUserSuccess);
                return;
            }

            if (response.find(toString(ResultStatus::DeleteUserFailed), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::DeleteUserFailed;
                statusStr = toString(ResultStatus::DeleteUserFailed);
                return;
            }

            //oneToOne Task cases
            if(response.find(toString(ResultStatus::GetOneToOneTasksSuccess), Qt::CaseSensitive) != std::string::npos)
            {
                status = ResultStatus::GetOneToOneTasksSuccess;
                statusStr = toString(ResultStatus::GetOneToOneTasksSuccess);

                nlohmann::json taskArray = nlohmann::json::parse(response);
                // 确保 tasks 是数组
                for (const auto& item : taskArray["tasks"]) {
                    Task task;
                    task.id = item.at("id").get<int>();
                    task.title = item.at("title").get<std::string>();
                    task.category = item.at("category").get<std::string>();
                    task.description = item.at("description").get<std::string>();
                    task.publish = item.at("publish").get<std::string>();
                    task.due = item.at("due").get<std::string>();
                    task.rating = item.at("rating").get<std::string>();
                    task.finishStatus = item.at("finishStatus").get<std::string>();
                    task.comment = item.at("comment").get<std::string>();
                    task.reviewString = item.at("reviewString").get<std::string>();
                    task.resultRating = item.at("resultRating").get<std::string>();
                    task.reviewStatus = item.at("reviewStatus").get<std::string>();

                    oneToOneTasks.push_back(task);
                }
                return;
            }

            if(response.find(toString(ResultStatus::GetOneToOneTaskSuccess), Qt::CaseSensitive) != std::string::npos)
            {
                status = ResultStatus::GetOneToOneTaskSuccess;
                statusStr = toString(ResultStatus::GetOneToOneTaskSuccess);

                nlohmann::json httpTask = nlohmann::json::parse(response);
                auto& item = httpTask["task"];

                Task task;
                task.id = item.at("id").get<int>();
                task.title = item.at("title").get<std::string>();
                task.category = item.at("category").get<std::string>();
                task.description = item.at("description").get<std::string>();
                task.publish = item.at("publish").get<std::string>();
                task.due = item.at("due").get<std::string>();
                task.rating = item.at("rating").get<std::string>();
                task.finishStatus = item.at("finishStatus").get<std::string>();
                task.comment = item.at("comment").get<std::string>();
                task.reviewString = item.at("reviewString").get<std::string>();
                task.resultRating = item.at("resultRating").get<std::string>();
                task.reviewStatus = item.at("reviewStatus").get<std::string>();

                oneToOneTasks.push_back(task);
                return;
            }


            // Register cases
            if(response.find(toString(ResultStatus::RegisterSuccess), Qt::CaseSensitive) != std::string::npos){
                status = ResultStatus::RegisterSuccess;
                statusStr = toString(ResultStatus::RegisterSuccess);
                return;
            }
            if(response.find(toString(ResultStatus::RegisterPending), Qt::CaseSensitive) != std::string::npos){
                status = ResultStatus::RegisterPending;
                statusStr = toString(ResultStatus::RegisterPending);
                return;
            }
            if (response.find(toString(ResultStatus::UserExist), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::UserExist;
                statusStr = toString(ResultStatus::UserExist);
                return;
            }
            if (response.find(toString(ResultStatus::EmailExist), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::EmailExist;
                statusStr = toString(ResultStatus::EmailExist);
                return;
            }
            if (response.find("not a valid email address", Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::EmailInvalid;
                statusStr = toString(ResultStatus::EmailInvalid);
                return;
            }

            // Login error cases
            if (response.find(toString(ResultStatus::UserOrEmailNotFound), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::UserOrEmailNotFound;
                statusStr = toString(ResultStatus::UserOrEmailNotFound);
                return;
            }
            if (response.find(toString(ResultStatus::PasswordIncorrect), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::PasswordIncorrect;
                statusStr = toString(ResultStatus::PasswordIncorrect);
                return;
            }
            if (response.find(toString(ResultStatus::AccountPending), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::AccountPending;
                statusStr = toString(ResultStatus::AccountPending);
                return;
            }
            if (response.find(toString(ResultStatus::AccountRejected), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::AccountRejected;
                statusStr = toString(ResultStatus::AccountRejected);
                return;
            }
            if (response.find(toString(ResultStatus::AccountBlacklisted), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::AccountBlacklisted;
                statusStr = toString(ResultStatus::AccountBlacklisted);
                return;
            }

            // Database upload cases
            if (response.find(toString(ResultStatus::DatabaseUploadSuccess), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::DatabaseUploadSuccess;
                statusStr = toString(ResultStatus::DatabaseUploadSuccess);
                return;
            }
            if (response.find(toString(ResultStatus::DatabaseUploadFailed), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::DatabaseUploadFailed;
                statusStr = toString(ResultStatus::DatabaseUploadFailed);
                return;
            }

            // Database download cases
            if (response.find(toString(ResultStatus::DatabaseFileDownloadSucess), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::DatabaseFileDownloadSucess;
                statusStr = toString(ResultStatus::DatabaseFileDownloadSucess);
                return;
            }
            if (response.find(toString(ResultStatus::DatabaseFileDownloadFailed), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::DatabaseFileDownloadFailed;
                statusStr = toString(ResultStatus::DatabaseFileDownloadFailed);
                return;
            }
            if (response.find(toString(ResultStatus::CreateDatabaseFileFailed), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::CreateDatabaseFileFailed;
                statusStr = toString(ResultStatus::CreateDatabaseFileFailed);
                return;
            }

            // OneToOne task
            if (response.find(toString(ResultStatus::AddOneToOneTaskSuccess), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::AddOneToOneTaskSuccess;
                statusStr = toString(ResultStatus::AddOneToOneTaskSuccess);

                nlohmann::json j = nlohmann::json::parse(response);

                if (j.contains("id") && j["id"].is_number_integer()) {
                    taskId = j["id"];
                }
                return;
            }

            if (response.find(toString(ResultStatus::UpdateOneToOneTaskSuccess), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::UpdateOneToOneTaskSuccess;
                statusStr = toString(ResultStatus::UpdateOneToOneTaskSuccess);
                return;
            }

            // Other cases
            if (response.find(toString(ResultStatus::CurlNotOK), Qt::CaseSensitive) != std::string::npos) {
                status = ResultStatus::CurlNotOK;
                statusStr = toString(ResultStatus::CurlNotOK);
                return;
            }

            // Default case
            status = ResultStatus::UnknownError;
            statusStr = toString(ResultStatus::UnknownError);
        }

        std::string toString(ResultStatus status)
        {
            switch (status) {
                // Register
                case ResultStatus::RegisterSuccess:
                    return "RegisterSuccess";
                case ResultStatus::RegisterPending:
                    return "RegisterPending";
                case ResultStatus::UserExist:
                    return "UserExist";
                case ResultStatus::EmailExist:
                    return "EmailExist";
                case ResultStatus::EmailInvalid:
                    return "EmailInvalid";

                // User
                case ResultStatus::GetAllUsersSuccess:
                    return "GetAllUsersSuccess";
                case ResultStatus::GetAllUsersFailed:
                    return "GetAllUsersFailed";
                case ResultStatus::AddUserSuccess:
                    return "AddUserSuccess";
                case ResultStatus::AddUserFailed:
                    return "AddUserFailed";
                case ResultStatus::ApproveUserSuccess:
                    return "ApproveUserSuccess";
                case ResultStatus::ApproveUserFailed:
                    return "ApproveUserFailed";
                case ResultStatus::RejectUserSuccess:
                    return "RejectUserSuccess";
                case ResultStatus::RejectUserFailed:
                    return "RejectUserFailed";
                case ResultStatus::BlacklistUserSuccess:
                    return "BlacklistUserSuccess";
                case ResultStatus::BlacklistUserFailed:
                    return "BlacklistUserFailed";
                case ResultStatus::DeleteUserSuccess:
                    return "DeleteUserSuccess";
                case ResultStatus::DeleteUserFailed:
                    return "DeleteUserFailed";


                // Login
                case ResultStatus::LoginSuccess:
                    return "LoginSuccess";
                case ResultStatus::UserOrEmailNotFound:
                    return "UserOrEmailNotFound";
                case ResultStatus::PasswordIncorrect:
                    return "PasswordIncorrect";
                case ResultStatus::AccountPending:
                    return "AccountPending";
                case ResultStatus::AccountRejected:
                    return "AccountRejected";
                case ResultStatus::AccountBlacklisted:
                    return "AccountBlacklisted";

                // Upload DB file
                case ResultStatus::DatabaseUploadSuccess:
                    return "DatabaseUploadSuccess";
                case ResultStatus::DatabaseUploadFailed:
                    return "DatabaseUploadFailed";

                // Download DB file
                case ResultStatus::CreateDatabaseFileFailed:
                    return "CreateDatabaseFileFailed";
                case ResultStatus::DatabaseFileDownloadSucess:
                    return "DatabaseFileDownloadSucess";
                case ResultStatus::DatabaseFileDownloadFailed:
                    return "DatabaseFileDownloadFailed";

                // OneToOne task
                case ClassScheduler::ResultStatus::GetOneToOneTaskSuccess:
                    return "GetOneToOneTaskSuccess";
                case ClassScheduler::ResultStatus::GetOneToOneTasksSuccess:
                    return "GetOneToOneTasksSuccess";
                case ClassScheduler::ResultStatus::AddOneToOneTaskSuccess:
                    return "AddOneToOneTaskSuccess";
                case ClassScheduler::ResultStatus::UpdateOneToOneTaskSuccess:
                    return "UpdateOneToOneTaskSuccess";

                // Feedback
                case ClassScheduler::ResultStatus::CreateFeedbackSuccess:
                    return "CreateFeedbackSuccess";
                case ClassScheduler::ResultStatus::CreateFeedbackFailed:
                    return "CreateFeedbackFailed";
                case ClassScheduler::ResultStatus::UpdateFeedbackStatusSuccess:
                    return "UpdateFeedbackStatusSuccess";
                case ClassScheduler::ResultStatus::UpdateFeedbackStatusFailed:
                    return "UpdateFeedbackStatusFailed";
                case ClassScheduler::ResultStatus::DeleteFeedbackSuccess:
                    return "DeleteFeedbackSuccess";
                case ClassScheduler::ResultStatus::DeleteFeedbackFailed:
                    return "DeleteFeedbackFailed";
                case ClassScheduler::ResultStatus::GetAllFeedbacksSuccess:
                    return "GetAllFeedbacksSuccess";
                case ClassScheduler::ResultStatus::GetAllFeedbacksFailed:
                    return "GetAllFeedbacksFailed";
                case ClassScheduler::ResultStatus::LikeFeedbackSuccess:
                    return "LikeFeedbackSuccess";
                case ClassScheduler::ResultStatus::LikeFeedbackFailed:
                    return "LikeFeedbackFailed";

                // Other
                case ResultStatus::CurlNotOK:
                    return "CurlNotOK";
                case ResultStatus::UnknownError:
                    return "UnknownError";

                default:
                    break;
            }
            return "UnknownError";
        }
    };

    using ScheduleClassResultInfos = std::vector<ScheduleClassResultInfo>;

    using TableDataCount = std::map<QString, int>; //QString tableName, int dataCount
    using AppSettings = std::vector<Setting>;
} // namespace ClassScheduler
