#include "TaskManager.h"
#include "NetworkManager.h"
#include "CoreFramework.h"
#include "Utils/ManagerUtils.h"
#include <iostream>

// 打印任务列表
void printTasks(const std::vector<Task>& tasks) {
    for (size_t i = 0; i < tasks.size(); ++i) {
        const auto& task = tasks[i];
        std::cout << "任务 " << (i + 1) << ":\n";
        std::cout << "  标题: " << task.title << "\n";
        std::cout << "  类别: " << task.category << "\n";
        std::cout << "  描述: " << task.description << "\n";
        std::cout << "  发布日期: " << task.publish << "\n";
        std::cout << "  截止日期: " << task.due << "\n";
        std::cout << "  评级: " << task.rating << "\n";
        std::cout << "  完成状态: " << task.finishStatus << "\n";
        std::cout << "  评论: " << (task.comment.empty() ? "无" : task.comment) << "\n";
        std::cout << "  审核字符串: " << (task.reviewString.empty() ? "无" : task.reviewString) << "\n";
        std::cout << "  结果评级: " << (task.resultRating.empty() ? "无" : task.resultRating) << "\n";
        std::cout << "  审核状态: " << task.reviewStatus << "\n";
        std::cout << "----------------------------------------\n";
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

void TaskManager::initTasks()
{
    mTasks = getTaskFromServer();
    initDailyTasks();
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
        std::cout << task.publish << " " << data << std::endl;
        if(task.publish == data)
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
            std::cout << result.statusStr << std::endl;
            std::cout << result.rawResponse << std::endl;
            if(result.status == ResultStatus::AddOneToOneTaskSuccess)
            {
                dailyTask.id = result.taskId;
            }
            mTasks.emplace_back(dailyTask);
        }
    }
    else
    {
        std::cout << "Daily tasks already exist" << std::endl;
    }
}

void TaskManager::refreshTaskStatus()
{
    const std::string currentDate = MUtils::getCurrentDate();
    const std::string startOfWeek = MUtils::getCurrentWeekFirstDay();
    const std::string endOfWeek = MUtils::getCurrentWeekLastDay();
    const std::string startOfMonth = MUtils::getCurrentMonthFirstDay();
    const std::string endOfMonth = MUtils::getCurrentMonthLastDay();

    for (auto& task : mTasks) {
        // 1. 先判断 shouldShow
        if (task.category == "今日") {
            task.shouldShow = (task.publish >= currentDate);
        } else if (task.category == "本周") {
            task.shouldShow = (task.publish >= startOfWeek);
        } else if (task.category == "本月") {
            task.shouldShow = (task.publish >= startOfMonth);
        } else {
            task.shouldShow = true;
        }

        // 2. 再判断过期状态
        task.isOverdue = false; // 默认未过期
        if (!task.due.empty()) {
            if (task.category == "今日") {
                task.isOverdue = (task.due < currentDate);
            } else if (task.category == "本周") {
                task.isOverdue = (task.due < endOfWeek);
            } else if (task.category == "本月") {
                task.isOverdue = (task.due < endOfMonth);
            }
        }
    }
}

bool TaskManager::updateTaskFinishStatus(int taskId, std::string commentString)
{
    auto coreFramework = mCoreFramework.lock();
    auto networkManager = coreFramework->getNetworkManager();

    for(auto& task : mTasks)
    {
        std::cout << "taskId: " << taskId << "task.id: " << task.id << std::endl;
        if(task.id == taskId)
        {
            task.comment = commentString;
            task.finishStatus = "已完成";

            auto result = networkManager->updateOneToOneTask(task.id, task);
            std::cout << result.statusStr << std::endl;
            std::cout << result.rawResponse << std::endl;
            if(result.status == ResultStatus::UpdateOneToOneTaskSuccess)
            {
                return true;
            }
        }
    }

    return false;
}

Tasks TaskManager::getTaskFromServer()
{
    if(auto coreFramework = mCoreFramework.lock())
    {
        if(auto networkManager = coreFramework->getNetworkManager())
        {
            auto response = networkManager->getAllOneToOneTasks();
            if(response.status == ResultStatus::GetOneToOneTasksSuccess)
            {
                return response.oneToOneTasks;
            }
            std::cout << response.rawResponse << std::endl;
            return {};
        }
    }
    std::cout << "no coreFramework or no networkManager" << std::endl;
    return {};
}

Task TaskManager::createTask(const TaskTemplate& tmpl, const std::string& date, const std::string& dueDate) {
    return Task(
        tmpl.title,
        tmpl.category,
        tmpl.description,
        date,           // publish 日期
        date,           // due 日期（与 publish 相同）
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
        {"登记当天工作", "今日", "登记当天必须完成的工作，完成后列在下方（如老师测试卷，家长是否做完，收费，催老师或者家长等）", "5"},
        {"登记明日跟进内容", "今日", "登记明天需要跟进的内容，列在下方（如老师测试卷，家长是否做完，收费，催老师或者家长等）", "5"}
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
        {"登记退课情况", "本周", "登记当天所有学生的退课情况，本周需完成", "1"},
        {"登记新好友名单", "本周", "登记当天新加好友名单及咨询情况", "2"},
        {"登记当天工作", "本周", "登记当天必须完成的工作，完成后列在下方（如老师测试卷，家长是否做完，收费，催老师或者家长等）", "3"},
        {"登记下周跟进内容", "本周", "登记明天需要跟进的内容，列在下方（如老师测试卷，家长是否做完，收费，催老师或者家长等）", "5"}
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
        {"登记退课情况", "本月", "登记当天所有学生的退课情况，本月需完成", "1"},
        {"登记新好友名单", "本月", "登记当天新加好友名单及咨询情况", "2"},
        {"登记当天工作", "本月", "登记当天必须完成的工作，完成后列在下方（如老师测试卷，家长是否做完，收费，催老师或者家长等）", "3"},
        {"登记下月跟进内容", "本月", "登记明天需要跟进的内容，列在下方（如老师测试卷，家长是否做完，收费，催老师或者家长等）", "4"}
    };

    std::vector<Task> tasks;
    for (const auto& tmpl : templates) {
        tasks.push_back(createTask(tmpl, publishDate, dueDate));
    }

    return tasks;
}
