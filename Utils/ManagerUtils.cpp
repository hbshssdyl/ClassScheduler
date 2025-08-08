#include "ManagerUtils.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

namespace ClassScheduler
{

std::string MUtils::getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(local, "%Y-%m-%d");
    return oss.str();
}

std::string MUtils::getCurrentWeekFirstDay() {
    std::time_t now = std::time(nullptr);
    std::tm local = *std::localtime(&now);
    int wday = (local.tm_wday == 0) ? 6 : (local.tm_wday - 1); // 周一为第一天
    local.tm_mday -= wday;
    std::mktime(&local);
    std::ostringstream oss;
    oss << std::put_time(&local, "%Y-%m-%d");
    return oss.str();
}

std::string MUtils::getCurrentWeekLastDay() {
    std::time_t now = std::time(nullptr);
    std::tm local = *std::localtime(&now);
    int wday = (local.tm_wday == 0) ? 6 : (local.tm_wday - 1);
    local.tm_mday += (6 - wday);
    std::mktime(&local);
    std::ostringstream oss;
    oss << std::put_time(&local, "%Y-%m-%d");
    return oss.str();
}

std::string MUtils::getCurrentMonthFirstDay() {
    std::time_t now = std::time(nullptr);
    std::tm local = *std::localtime(&now);
    local.tm_mday = 1;
    std::mktime(&local);
    std::ostringstream oss;
    oss << std::put_time(&local, "%Y-%m-%d");
    return oss.str();
}

std::string MUtils::getCurrentMonthLastDay() {
    std::time_t now = std::time(nullptr);
    std::tm local = *std::localtime(&now);
    local.tm_mday = 1;
    local.tm_mon += 1;  // 下个月
    local.tm_mday -= 1; // 回到上个月最后一天
    std::mktime(&local);
    std::ostringstream oss;
    oss << std::put_time(&local, "%Y-%m-%d");
    return oss.str();
}

}
