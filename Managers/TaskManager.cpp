#include "TaskManager.h"
#include "NetworkManager.h"
#include "CoreFramework.h"
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
    initialize();
}

void TaskManager::initialize()
{
    auto tasks = generateDailyTasks();
    printTasks(tasks);
}

void TaskManager::initTasks()
{
    mTasks = getTaskFromServer();
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
        }
    }
}

std::string TaskManager::getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(local, "%Y-%m-%d");
    return oss.str();
}

Task TaskManager::createTask(const TaskTemplate& tmpl, const std::string& date) {
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
    std::string date = getCurrentDate();

    // 定义任务模板
    std::vector<TaskTemplate> templates = {
        {"登记退课情况", "本日", "登记当天所有学生的退课情况，本日需完成", "5"},
        {"登记新好友名单", "本日", "登记当天新加好友名单及咨询情况", "5"},
        {"登记当天工作", "本日", "登记当天必须完成的工作，完成后列在下方（如老师测试卷，家长是否做完，收费，催老师或者家长等）", "5"},
        {"登记明日跟进内容", "本日", "登记明天需要跟进的内容，列在下方（如老师测试卷，家长是否做完，收费，催老师或者家长等）", "5"}
    };

    std::vector<Task> tasks;
    for (const auto& tmpl : templates) {
        tasks.push_back(createTask(tmpl, date));
    }

    return tasks;
}
