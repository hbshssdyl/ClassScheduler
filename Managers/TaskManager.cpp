#include "TaskManager.h"
#include "NetworkManager.h"
#include "CoreFramework.h"
#include "Utils/ManagerUtils.h"
#include <iostream>

// 打印任务列表
void printTasks(const std::vector<Task>& tasks) {
    for (size_t i = 0; i < tasks.size(); ++i) {
        const auto& task = tasks[i];
        LOG_INFO("任务 " + std::to_string(i + 1));
        LOG_INFO(" 标题: " +task.title);
        LOG_INFO(" 类别: " +task.category);
        LOG_INFO(" 描述: " +task.description);
        LOG_INFO(" 发布日期: " +task.publish);
        LOG_INFO(" 截止日期: " +task.due);
        LOG_INFO(" 评级: " +task.rating);
        LOG_INFO(" 完成状态: " +task.finishStatus);
        LOG_INFO(" 评论: " +(task.comment.empty() ? "无" : task.comment));
        LOG_INFO(" 审核字符串: " +(task.reviewString.empty() ? "无" : task.reviewString));
        LOG_INFO(" 结果评级: " +(task.resultRating.empty() ? "无" : task.resultRating));
        LOG_INFO(" 审核状态: " +task.reviewStatus);
        LOG_INFO("----------------------------------------");
    }
}

TaskManager::TaskManager(CoreFrameworkPtr coreFramework)
    : mCoreFramework(coreFramework)
{
}

void TaskManager::initialize()
{
    initTasks();
}

void TaskManager::refreshDataFromServer()
{
    initTasks();
}

void TaskManager::initTasks()
{
    auto [tasks, status] = getTaskFromServer();
    mTasks = tasks;
    if(status == ResultStatus::GetOneToOneTasksSuccess)
    {
        initDailyTasks();
        initWeeklyTasks();
        initMonthlyTasks();
        refreshTaskStatus();
    }
}

Tasks TaskManager::getTasks()
{
    return mTasks;
}

void TaskManager::initDailyTasks()
{
    auto coreFramework = mCoreFramework.lock();
    auto networkManager = coreFramework->getNetworkManager();
    std::string data = MUtils::getCurrentDate();
    bool needNewDailyTasks = true;
    for(auto& task : mTasks)
    {
        //LOG_INFO(task.publish + " " + data + " " + task.category);
        if(task.publish == data && task.category == "今日")
        {
            needNewDailyTasks = false;
            break;
        }
    }
    if(needNewDailyTasks)
    {
        auto dailyTasks = generateDailyTasks();
        for(auto& dailyTask : dailyTasks)
        {
            auto result = networkManager->createOneToOneTask(dailyTask);
            LOG_INFO(result.statusStr);
            LOG_INFO(result.rawResponse);
            if(result.status == ResultStatus::AddOneToOneTaskSuccess)
            {
                dailyTask.id = result.taskId;
            }
            mTasks.emplace_back(dailyTask);
        }
    }
    else
    {
        LOG_INFO("Daily tasks already exist");
    }
}

void TaskManager::initWeeklyTasks()
{
    auto coreFramework = mCoreFramework.lock();
    auto networkManager = coreFramework->getNetworkManager();
    std::string weekStart = MUtils::getCurrentWeekFirstDay();
    bool needNewWeeklyTasks = true;
    for(auto& task : mTasks)
    {
        //LOG_INFO(task.publish +" " +weekStart);
        if(task.publish == weekStart && task.category == "本周")
        {
            needNewWeeklyTasks = false;
            break;
        }
    }
    if(needNewWeeklyTasks)
    {
        auto weeklyTasks = generateWeeklyTasks();
        for(auto& weeklyTask : weeklyTasks)
        {
            auto result = networkManager->createOneToOneTask(weeklyTask);
            LOG_INFO(result.statusStr);
            LOG_INFO(result.rawResponse);
            if(result.status == ResultStatus::AddOneToOneTaskSuccess)
            {
                weeklyTask.id = result.taskId;
            }
            mTasks.emplace_back(weeklyTask);
        }
    }
    else
    {
        LOG_INFO("Weekly tasks already exist");
    }
}

void TaskManager::initMonthlyTasks()
{
    auto coreFramework = mCoreFramework.lock();
    auto networkManager = coreFramework->getNetworkManager();
    std::string monthStart = MUtils::getCurrentMonthFirstDay();
    bool needNewMonthlyTasks = true;
    for(auto& task : mTasks)
    {
        //LOG_INFO(task.publish +" " +monthStart);
        if(task.publish == monthStart && task.category == "本月")
        {
            needNewMonthlyTasks = false;
            break;
        }
    }
    if(needNewMonthlyTasks)
    {
        auto monthlyTasks = generateMonthlyTasks();
        for(auto& monthlyTask : monthlyTasks)
        {
            auto result = networkManager->createOneToOneTask(monthlyTask);
            LOG_INFO(result.statusStr);
            LOG_INFO(result.rawResponse);
            if(result.status == ResultStatus::AddOneToOneTaskSuccess)
            {
                monthlyTask.id = result.taskId;
            }
            mTasks.emplace_back(monthlyTask);
        }
    }
    else
    {
        LOG_INFO("Monthly tasks already exist");
    }
}

void TaskManager::refreshTaskStatus()
{
    const std::string currentDate = MUtils::getCurrentDate();
    for (auto& task : mTasks) {
        task.isOverdue = task.due < currentDate;
        if(task.isOverdue)
        {
            task.finishStatus = "已过期";
        }
    }
}

bool TaskManager::updateTaskFinishStatus(int taskId, std::string commentString)
{
    auto coreFramework = mCoreFramework.lock();
    auto networkManager = coreFramework->getNetworkManager();
    for(auto& task : mTasks)
    {
        LOG_INFO("taskId: " + std::to_string(taskId) + "task.id: " + std::to_string(task.id));
        if(task.id == taskId)
        {
            task.comment = commentString;
            task.finishStatus = "已完成";
            auto result = networkManager->updateOneToOneTask(task.id, task);
            LOG_INFO(result.statusStr);
            LOG_INFO(result.rawResponse);
            if(result.status == ResultStatus::UpdateOneToOneTaskSuccess)
            {
                return true;
            }
        }
    }
    return false;
}

bool TaskManager::updateTaskReviewStatus(int taskId, std::string resultRating, std::string reviewString)
{
    auto coreFramework = mCoreFramework.lock();
    auto networkManager = coreFramework->getNetworkManager();
    for(auto& task : mTasks)
    {
        LOG_INFO("taskId: " + std::to_string(taskId) +"task.id: " + std::to_string(task.id));
        if(task.id == taskId)
        {
            task.resultRating = resultRating;
            task.reviewString = reviewString;
            task.reviewStatus = "已审核";
            auto result = networkManager->updateOneToOneTask(task.id, task);
            LOG_INFO(result.statusStr);
            LOG_INFO(result.rawResponse);
            if(result.status == ResultStatus::UpdateOneToOneTaskSuccess)
            {
                return true;
            }
        }
    }
    return false;
}

bool TaskManager::addTask(Task task)
{
    auto coreFramework = mCoreFramework.lock();
    auto networkManager = coreFramework->getNetworkManager();
    auto result = networkManager->createOneToOneTask(task);
    LOG_INFO(result.statusStr);
    LOG_INFO(result.rawResponse);
    if(result.status == ResultStatus::AddOneToOneTaskSuccess)
    {
        mTasks.emplace_back(task);
        return true;
    }
    return false;
}

std::pair<Tasks, ResultStatus> TaskManager::getTaskFromServer()
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            auto response = networkManager->getAllOneToOneTasks();
            if(response.status == ResultStatus::GetOneToOneTasksSuccess)
            {
                return {response.oneToOneTasks, response.status};
            }
            LOG_INFO(response.rawResponse);
            return {response.oneToOneTasks, response.status};
        }
    }
    LOG_INFO("no coreFramework or no networkManager");
    return {{}, ResultStatus::UnknownError};
}

Task TaskManager::createTask(const TaskTemplate& tmpl, const std::string& date, const std::string& dueDate) {
    return Task(
        tmpl.title,
        tmpl.category,
        tmpl.description,
        date, // publish 日期
        dueDate, // due 日期
        tmpl.rating,
        "未完成",
        "",
        "",
        "",
        "未审核"
        );
}

// 生成每日任务
std::vector<Task> TaskManager::generateDailyTasks() {
    std::string publishDate = MUtils::getCurrentDate();
    std::string dueDate = publishDate;
    // 定义任务模板
    std::vector<TaskTemplate> templates = {
        {"登记退课情况", "今日", "登记当天所有学生的退课情况，今日需完成", "5"},
        {"登记新好友名单", "今日", "登记当天新加好友名单及咨询情况", "5"},
        {"登记老师停课，换课情况", "今日", "登记当天的所有老师停课，换课情况，不要遗漏", "3"},
        {"登记家长停课，换课情况", "今日", "登记当天的所有家长停课，换课情况，不要遗漏", "3"},
        {"总结当天必须完成的工作", "今日", "登记当天必须完成的工作，完成后列在下方（如老师测试卷，家长是否做完，收费，催老师或者家长等）", "2"},
        {"总结当天未完成的工作", "今日", "登记当天尚未完成的工作，完成后列在下方（如老师测试卷，家长是否做完，收费，催老师或者家长等）", "5"},
        {"整理明天需要跟进的内容", "今日", "整理明天需要跟进的内容（如老师测试卷，家长是否做完，收费，催老师或者家长等）", "3"}
    };
    std::vector<Task> tasks;
    for (const auto& tmpl : templates) {
        tasks.push_back(createTask(tmpl, publishDate, dueDate));
    }
    return tasks;
}

// 生成每周任务
std::vector<Task> TaskManager::generateWeeklyTasks() {
    std::string publishDate = MUtils::getCurrentWeekFirstDay();
    std::string dueDate = MUtils::getCurrentWeekLastDay();
    // 定义任务模板
    std::vector<TaskTemplate> templates = {
        {"整理本周跟进内容", "本周", "整理本周需要跟进的内容，列在下方（如老师测试卷，家长是否做完，收费，催老师或者家长等）", "5"}
    };
    std::vector<Task> tasks;
    for (const auto& tmpl : templates) {
        tasks.push_back(createTask(tmpl, publishDate, dueDate));
    }
    return tasks;
}

// 生成每月任务
std::vector<Task> TaskManager::generateMonthlyTasks() {
    std::string publishDate = MUtils::getCurrentMonthFirstDay();
    std::string dueDate = MUtils::getCurrentMonthLastDay();
    // 定义任务模板
    std::vector<TaskTemplate> templates = {
        {"整理本月跟进内容", "本月", "整理本月需要跟进的内容，列在下方（如老师测试卷，家长是否做完，收费，催老师或者家长等）", "4"}
    };
    std::vector<Task> tasks;
    for (const auto& tmpl : templates) {
        tasks.push_back(createTask(tmpl, publishDate, dueDate));
    }
    return tasks;
}
