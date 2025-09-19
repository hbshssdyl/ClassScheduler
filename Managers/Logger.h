#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <mutex>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

class Logger {
public:
    static Logger& instance();

    // 初始化日志系统
    void init(const std::string &filename = "application.log",
              LogLevel consoleLevel = LOG_DEBUG,
              LogLevel fileLevel = LOG_DEBUG);

    // 设置日志级别
    void setConsoleLogLevel(LogLevel level);
    void setFileLogLevel(LogLevel level);

    // 日志输出方法
    void debug(const std::string &message, const char *file, const char *function, int line);
    void info(const std::string &message, const char *file, const char *function, int line);
    void warning(const std::string &message, const char *file, const char *function, int line);
    void error(const std::string &message, const char *file, const char *function, int line);

    // 关闭日志系统
    void shutdown();

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void writeLog(LogLevel level, const std::string &message, const char *file, const char *function, int line);
    std::string levelToString(LogLevel level);
    std::string getCurrentTime();
    std::string getFileName(const std::string &fullPath);

    std::ofstream logFile;
    LogLevel consoleLevel;
    LogLevel fileLevel;
    bool initialized;
    std::mutex logMutex;
};

// 宏定义方便使用
#define LOG_DEBUG(msg) Logger::instance().debug(msg, __FILE__, __FUNCTION__, __LINE__)
#define LOG_INFO(msg) Logger::instance().info(msg, __FILE__, __FUNCTION__, __LINE__)
#define LOG_WARNING(msg) Logger::instance().warning(msg, __FILE__, __FUNCTION__, __LINE__)
#define LOG_ERROR(msg) Logger::instance().error(msg, __FILE__, __FUNCTION__, __LINE__)

#endif // LOGGER_H
