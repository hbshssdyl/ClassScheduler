#include "DataUtils.h"
#include "ControllerUtils.h"

namespace fs = std::filesystem;
namespace ClassScheduler
{

QString toOperateModeString(OperateMode mode)
{
    switch (mode)
    {
    case OperateMode::None:
        return "None";
    case OperateMode::FileView:
        return "更新数据库";
    case OperateMode::WelcomePage:
        return "欢迎使用";
    case OperateMode::SearchClassInfo:
        return "课程信息";
    case OperateMode::SearchTeacherInfo:
        return "教师信息";
    case OperateMode::SearchStudentInfo:
        return "学生信息";
    case OperateMode::ScheduleClass:
        return "排课";
    case OperateMode::TeacherEvaluation:
        return "老师评价";
    case OperateMode::TaskAssistantView:
        return "一对一助理";
    case OperateMode::TaskManagerView:
        return "一对一管理";
    case OperateMode::FeedbackApp:
        return "吐槽软件";
    case OperateMode::FeedbackCompany:
        return "吐槽公司";
    case OperateMode::AccountView:
        return "账号信息";
    default:
        return "default";
    }
}

QString getIconNameByOperateMode(OperateMode mode)
{
    switch (mode)
    {
    case OperateMode::None:
    case OperateMode::WelcomePage:
        return "";
    case OperateMode::FileView:
        return "database.svg";
    case OperateMode::SearchClassInfo:
        return "folder.svg";
    case OperateMode::SearchTeacherInfo:
        return "study.svg";
    case OperateMode::SearchStudentInfo:
        return "user.svg";
    case OperateMode::ScheduleClass:
        return "tag.svg";
    case OperateMode::TeacherEvaluation:
        return "QA.svg";
    case OperateMode::TaskAssistantView:
        return "task.svg";
    case OperateMode::TaskManagerView:
        return "manage.svg";
    case OperateMode::FeedbackApp:
        return "question.svg";
    case OperateMode::FeedbackCompany:
        return "notification.svg";
    case OperateMode::AccountView:
        return "phone.svg";
    default:
        return "default";
    }
}

QVariantMap getClassListInfo(int id, ClassInfo classInfo)
{
    return QVariantMap{{ "id", QString::number(id) },
                       { "date", classInfo.date },
                       { "weekend", classInfo.weekend },
                       { "studentName", classInfo.studentName },
                       { "school", classInfo.school },
                       { "studentPhoneNubmer", classInfo.studentPhoneNubmer },
                       { "grade", classInfo.grade },

                       { "suject", classInfo.suject },
                       { "time", classInfo.time },
                       { "teacherNickName", classInfo.teacherNickName },
                       { "learningType", classInfo.learningType },
                       { "courseTime", classInfo.courseTime },
                       { "studentFee", classInfo.studentFee },

                       { "studentTotalFee", classInfo.studentTotalFee },
                       { "teacherName", classInfo.teacherName },
                       { "teacherFee", classInfo.teacherFee },
                       { "gotMoney", classInfo.gotMoney },
                       { "payType", classInfo.payType },
                       { "payDate", classInfo.payDate }};
}

bool isCurrentMonth(const QString& dateStr) {
    //return true;
    QDate inputDate = QDate::fromString(dateStr, "yyyy-MM-dd");
    if (!inputDate.isValid()) return false;
    QDate currentDate = QDate::currentDate();
    return (inputDate.year() == currentDate.year() && inputDate.month() == currentDate.month() - 2);
}

//For controller.cpp
void CUtils::updateActionItemsList(QVariantList& data, const OperateMode& selectedMode, const OperateModes& actionItems)
{
    for(auto activeItem : actionItems)
    {
        data.append(QVariantMap{ { "actionName", toOperateModeString(activeItem) },
                                { "actionIcon", getIconNameByOperateMode(activeItem) },
                                { "OperateMode", static_cast<int>(activeItem) },
                                { "isSelected", activeItem == selectedMode }});
    }
}

QString CUtils::toString(OperateMode mode)
{
    switch (mode)
    {
    case OperateMode::None:
        return "None";
    case OperateMode::LoginView:
        return "LoginView";
    case OperateMode::FileView:
        return "FileView";
    case OperateMode::WelcomePage:
        return "WelcomePage";
    case OperateMode::SearchClassInfo:
        return "SearchClassInfo";
    case OperateMode::SearchTeacherInfo:
        return "SearchTeacherInfo";
    case OperateMode::SearchStudentInfo:
        return "SearchStudentInfo";
    case OperateMode::ScheduleClass:
        return "ScheduleClass";
    case OperateMode::TaskAssistantView:
        return "TaskAssistantView";
    case OperateMode::TaskManagerView:
        return "TaskManagerView";
    case OperateMode::TeacherEvaluation:
        return "TeacherEvaluation";
    case OperateMode::FeedbackApp:
        return "FeedbackApp";
    case OperateMode::FeedbackCompany:
        return "FeedbackCompany";
    case OperateMode::AccountView:
        return "AccountView";
    default:
        return "default";
    }
}

//For SearchClassInfoController.cpp
void CUtils::updateClassInfoList(QVariantMap& data, ClassInfos& classInfos)
{
    QVariantList classInfoHeader;
    QVariantList classInfoList;
    int id = 1;

    classInfoHeader.append("序号");
    for(auto header : validExcelClassHeader)
    {
        classInfoHeader.append(header);
    }

    for(auto& classInfo : classInfos)
    {
        classInfoList.append(classInfo.toInfosList(QString::number(id++)));
    }

    data.insert("classInfoHeader", classInfoHeader);
    data.insert("classInfoList", classInfoList);
}

void CUtils::doSearchClassInfos(ClassInfos& allInfos, ClassInfos& searchInfos, QString searchString)
{
    for(auto& info : allInfos)
    {
        if(info.isAllContains(searchString))
        {
            searchInfos.emplace_back(info);
        }
    }
}

//For SearchTeacherInfoController.cpp
void CUtils::updateTeacherInfoList(QVariantMap& data, TeacherInfos& teacherInfos)
{
    if(teacherInfos.size() == 0)
    {
        return;
    }
    QVariantList teacherInfoHeader;
    QVariantList teacherInfoList;

    //update teacherInfoHeader
    teacherInfoHeader.append("序号");
    for(auto header : validTeacherHeader)
    {
        teacherInfoHeader.append(header);
    }

    //update teacherInfoList
    int id = 1;
    for(auto& teacherInfo : teacherInfos)
    {
        QVariantList sujectStudentInfoList;
        for(auto& info : teacherInfo.sujectStudentCounts)
        {
            sujectStudentInfoList.append(info.toMapStyle());
        }
        teacherInfoList.append(QVariantMap{ { "basicInfo", teacherInfo.toInfosList(QString::number(id++)) },
                                            { "studentInfo", sujectStudentInfoList } });
    }

    data.insert("teacherInfoHeader", teacherInfoHeader);
    data.insert("teacherInfoList", teacherInfoList);

    //update studentBasicInfo
    data.insert("maxStudentCount", teacherInfos[0].teacherStudentCountBasicInfo.maxKeyCount);
    data.insert("minYearMonth", teacherInfos[0].teacherStudentCountBasicInfo.minYearMonth);
    data.insert("maxYearMonth", teacherInfos[0].teacherStudentCountBasicInfo.maxYearMonth);
}

void CUtils::doSearchTeacherInfos(TeacherInfos& allInfos, TeacherInfos& searchInfos, QString searchString)
{
    for(auto& info : allInfos)
    {
        if(info.isAllContains(searchString))
        {
            searchInfos.emplace_back(info);
        }
    }
}

//For SearchStudentInfoController.cpp
void CUtils::updateStudentInfoList(QVariantMap& data, StudentInfos& studentInfos)
{
    if(studentInfos.size() == 0)
    {
        return;
    }
    QVariantList studentInfoHeader;
    QVariantList studentInfoList;

    //update studentInfoHeader
    studentInfoHeader.append("序号");
    for(auto header : validStudentHeader)
    {
        studentInfoHeader.append(header);
    }

    //update studentInfoList
    int id = 1;
    for(auto& studentInfo : studentInfos)
    {
        QVariantList sujectClassInfoList;
        for(auto& info : studentInfo.sujectClassCounts)
        {
            sujectClassInfoList.append(info.toMapStyle());
        }
        studentInfoList.append(QVariantMap{ { "basicInfo", studentInfo.toInfosList(QString::number(id++)) },
                                            { "classInfo", sujectClassInfoList } });
    }

    data.insert("studentInfoHeader", studentInfoHeader);
    data.insert("studentInfoList", studentInfoList);

    //update studentBasicInfo
    data.insert("maxClassCount", studentInfos[0].studentClassCountBasicInfo.maxKeyCount);
    data.insert("minYearMonth", studentInfos[0].studentClassCountBasicInfo.minYearMonth);
    data.insert("maxYearMonth", studentInfos[0].studentClassCountBasicInfo.maxYearMonth);
}

void CUtils::doSearchStudentInfos(StudentInfos& allInfos, StudentInfos& searchInfos, QString searchString)
{
    for(auto& info : allInfos)
    {
        if(info.isAllContains(searchString))
        {
            searchInfos.emplace_back(info);
        }
    }
}

//For ScheduleClassController.cpp
bool CUtils::isTimeOverlap(const QString& timeRange1, const QString& timeRange2) {
    QStringList range1 = timeRange1.split('-');
    QStringList range2 = timeRange2.split('-');

    if (range1.size() != 2 || range2.size() != 2) {
        return false;
    }

    QTime start1 = QTime::fromString(range1[0], "hh:mm");
    QTime end1 = QTime::fromString(range1[1], "hh:mm");
    QTime start2 = QTime::fromString(range2[0], "hh:mm");
    QTime end2 = QTime::fromString(range2[1], "hh:mm");

    if (!start1.isValid() || !end1.isValid() || !start2.isValid() || !end2.isValid()) {
        std::cout << "Invalid time format" << std::endl;
        return false; // Invalid time format
    }

    if(start1 <= start2 && end1 >= end2)
    {
        return true;
    }

    if(start2 <= start1 && end2 >= end1)
    {
        return true;
    }

    if(start2 >= start1 && start2 < end1 && end2 >= end1)
    {
        return true;
    }

    if(start1 >= start2 && start1 < end2 && end1 >= end2)
    {
        return true;
    }

    return false;
}

void CUtils::updateScheduleClassResultsList(QList<QList<QVariantMap>>& data, ScheduleClassInputInfo& inputInfos, ClassInfos& classInfosList)
{
    map<QString, bool> isValidTeacher;
    map<QString, bool> isValidTeacherSuject;

    // 如果当前老师从来没有教过输入的科目，则默认该老师不可能被安排新课
    for(auto& classInfo : classInfosList)
    {
        if(isValidTeacherSuject.count(classInfo.teacherName) == 1)
        {
            if(isValidTeacherSuject[classInfo.teacherName])
            {
                continue;
            }
        }

        isValidTeacherSuject[classInfo.teacherName] = false;
        if(classInfo.suject == inputInfos.suject)
        {
            isValidTeacherSuject[classInfo.teacherName] = true;
        }
    }

    // 如果当前老师当月在输入的时间上有课的话，就认为该老师不可能被安排新课
    for(auto& classInfo : classInfosList)
    {

        if(!isCurrentMonth(classInfo.date))
        {
            continue;
        }

        if(isValidTeacher.count(classInfo.teacherName) == 1)
        {
            if(!isValidTeacher[classInfo.teacherName])
            {
                continue;
            }
        }

        isValidTeacher[classInfo.teacherName] = isValidTeacherSuject[classInfo.teacherName];
        if(classInfo.weekend == inputInfos.week)
        {
            if(isTimeOverlap(classInfo.time, inputInfos.timeRange))
            {
                std::cout << "TimeOverlap: classInfo.time: " << classInfo.time.toStdString() << ", inputInfos.timeRange: " << inputInfos.timeRange.toStdString() << std::endl;
                isValidTeacher[classInfo.teacherName] = false;
            }
        }
    }

    ScheduleClassResultInfos resultInfoList;
    for(auto& classInfo : classInfosList)
    {
        if(!isCurrentMonth(classInfo.date))
        {
            continue;
        }

        if(isValidTeacher[classInfo.teacherName])
        {
            bool isNewTeacher = true;
            for(auto& result : resultInfoList)
            {
                if(result.teacherName == classInfo.teacherName)
                {
                    isNewTeacher = false;
                    result.saveValue(classInfo.teacherNickName, result.teacherNickNames);
                    result.saveValue(classInfo.grade + "_" + classInfo.teacherFee, result.teacherGradeFees);

                    if(classInfo.weekend == inputInfos.week)
                    {
                        //result.saveValue(classInfo.grade, result.teacherWorkGrade);
                        result.saveValue(classInfo.time, result.teacherWorkTime, false);
                    }
                    break;
                }
            }
            if(isNewTeacher)
            {
                ScheduleClassResultInfo info;
                info.teacherName = classInfo.teacherName;
                info.week = inputInfos.week;
                info.teacherNickNames.emplace_back(classInfo.teacherNickName);
                info.teacherGradeFees.emplace_back(classInfo.grade + "_" + classInfo.teacherFee);

                if(classInfo.weekend == inputInfos.week)
                {
                    info.teacherWorkTime.emplace_back(classInfo.time);
                    //info.teacherWorkGrade.emplace_back(classInfo.grade);
                }
                resultInfoList.emplace_back(info);
            }
        }
    }

    for(auto& result : resultInfoList)
    {
        result.generateValidWorkTime();
        result.generateTeacherFreeTime();
        result.generateValidFreeTime(inputInfos.timeLength);
        result.sortInfos();
        result.generateInfoString();
    }

    for(auto& result : resultInfoList)
    {
        if(!result.isValid(inputInfos.timeRange))
        {
            continue;
        }

        QList<QVariantMap> tmp;
        tmp.append(QVariantMap{ { "label", "姓　　名： " },
                                 { "value", result.teacherName } });

        tmp.append(QVariantMap{ { "label", "昵　　称： " },
                                 { "value", result.strTeacherNickNames } });

        tmp.append(QVariantMap{ { "label", "年级费用： " },
                                 { "value", result.strTeacherGradeFees } });

        // tmp.append(QVariantMap{ { "label", "教过年级： " },
        //                          { "value", result.strTeacherWorkTime } });

        tmp.append(QVariantMap{ { "label", "有课时间： " },
                                 { "value", result.strTeacherValidWorkTime } });

        tmp.append(QVariantMap{ { "label", "无课时间： " },
                                 { "value", result.strTeacherValidFreeTime } });
        data.append(tmp);
    }
}

// for TaskController
void CUtils::updateTasksList(QVariantList& data, Tasks& tasks)
{
    for(const auto& task : tasks)
    {
        data.append(QVariantMap{
            { "taskId", task.id },
            { "isOverdue", task.isOverdue },
            { "title", QString::fromStdString(task.title) },
            { "category", QString::fromStdString(task.category) },
            { "description", QString::fromStdString(task.description) },
            { "publish", QString::fromStdString(task.publish) },
            { "due", QString::fromStdString(task.due) },
            { "rating", QString::fromStdString(task.rating) },
            { "finishStatus", QString::fromStdString(task.finishStatus) },
            { "comment", QString::fromStdString(task.comment) },
            { "reviewString", QString::fromStdString(task.reviewString) },
            { "resultRating", QString::fromStdString(task.resultRating) },
            { "reviewStatus", QString::fromStdString(task.reviewStatus) }
        });
    }
}

// for AccountViewController
void CUtils::updateAccountsList(QVariantList& pendingData, QVariantList& finishedData, QVariantList& blacklistData, UserInfos& accounts)
{
    for(const auto& account : accounts)
    {
        auto data = QVariantMap{
                { "accountId", account.id },
                { "username", QString::fromStdString(account.name) },
                { "email", QString::fromStdString(account.email) },
                { "password", QString::fromStdString(account.password) },
                { "role", QString::fromStdString(account.roleStr) },
                { "account_status", QString::fromStdString(account.accountStatus) }};
        if(account.accountStatus == toAccountStatus(AccountStatus::REGISTERED))
        {
            pendingData.append(data);
        }
        else if(account.accountStatus == toAccountStatus(AccountStatus::BLACKLISTED))
        {
            blacklistData.append(data);
        }
        else
        {
            finishedData.append(data);
        }

    }
}

// for FeedbackController
void CUtils::updatefeedbackInfosList(QVariantList& feedbackAppList, QVariantList& feedbackCompanyList, FeedbackInfos& feedbackInfos, bool isSupperAdmin)
{
    for(const auto& feedbackInfo : feedbackInfos)
    {
        if(feedbackInfo.type == FeedbackType::AppIssue)
        {
            auto data = QVariantMap{
                { "feedbackId", feedbackInfo.id },
                { "username", QString::fromStdString(feedbackInfo.feedbackUsername) },
                { "isAnonymous", feedbackInfo.userType == FeedbackUserType::Anonymous },
                { "likeCount", feedbackInfo.likeCount },

                { "showAdminInfo", isSupperAdmin },
                { "feedbackStatus", QString::fromStdString(to_string(feedbackInfo.feedbackStatus)) },
                { "realUsername", QString::fromStdString(feedbackInfo.realUsername) },
                { "realEmail", QString::fromStdString(feedbackInfo.realEmail) },

                { "dataAndTime", QString::fromStdString(feedbackInfo.dataAndTime) },
                { "message", QString::fromStdString(feedbackInfo.message) }};
            feedbackAppList.append(data);
        }

        if(feedbackInfo.type == FeedbackType::CompanyIssue)
        {
            auto data = QVariantMap{
                { "feedbackId", feedbackInfo.id },
                { "username", QString::fromStdString(feedbackInfo.feedbackUsername) },
                { "isAnonymous", feedbackInfo.userType == FeedbackUserType::Anonymous },
                { "likeCount", feedbackInfo.likeCount },

                { "showAdminInfo", isSupperAdmin },
                { "feedbackStatus", QString::fromStdString(to_string(feedbackInfo.feedbackStatus)) },
                { "realUsername", QString::fromStdString(feedbackInfo.realUsername) },
                { "realEmail", QString::fromStdString(feedbackInfo.realEmail) },

                { "dataAndTime", QString::fromStdString(feedbackInfo.dataAndTime) },
                { "message", QString::fromStdString(feedbackInfo.message) }};
            feedbackCompanyList.append(data);
        }
    }
}

} // namespace PictureManager
