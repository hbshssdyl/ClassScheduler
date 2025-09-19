#include "Logger.h"
#include <filesystem>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define mkdir _mkdir
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : consoleLevel(LOG_DEBUG), fileLevel(LOG_DEBUG), initialized(false) {
}

Logger::~Logger() {
    shutdown();
}

void Logger::init(const std::string &filename, LogLevel consoleLevel, LogLevel fileLevel) {
    std::lock_guard<std::mutex> lock(logMutex);

    if (initialized) {
        return;
    }

    this->consoleLevel = consoleLevel;
    this->fileLevel = fileLevel;

    // 创建目录（如果不存在）
    size_t lastSlash = filename.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        std::string dirPath = filename.substr(0, lastSlash);
#ifdef _WIN32
        mkdir(dirPath.c_str());
#else
        mkdir(dirPath.c_str(), 0777);
#endif
    }

    logFile.open(filename, std::ios::out | std::ios::app);
    if (logFile.is_open()) {
        logFile << "========================================\n";
        logFile << "Application started at: " << getCurrentTime() << "\n";
        logFile << "========================================\n\n";
        initialized = true;

        LOG_INFO("Logger initialized successfully");
    } else {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

void Logger::setConsoleLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(logMutex);
    consoleLevel = level;
}

void Logger::setFileLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(logMutex);
    fileLevel = level;
}

void Logger::debug(const std::string &message, const char *file, const char *function, int line) {
    writeLog(LOG_DEBUG, message, file, function, line);
}

void Logger::info(const std::string &message, const char *file, const char *function, int line) {
    writeLog(LOG_INFO, message, file, function, line);
}

void Logger::warning(const std::string &message, const char *file, const char *function, int line) {
    writeLog(LOG_WARNING, message, file, function, line);
}

void Logger::error(const std::string &message, const char *file, const char *function, int line) {
    writeLog(LOG_ERROR, message, file, function, line);
}

void Logger::writeLog(LogLevel level, const std::string &message, const char *file, const char *function, int line) {
    std::lock_guard<std::mutex> lock(logMutex);

    if (!initialized) {
        // 如果没有初始化，默认输出到控制台
        std::cout << "[" << getCurrentTime() << "] ["
                  << levelToString(level) << "] " << message << std::endl;
        return;
    }

    std::string fileName = getFileName(file);
    std::string logMessage = "[" + getCurrentTime() + "] [" +
                             levelToString(level) + "] [" +
                             fileName + ":" + std::to_string(line) + " " +
                             function + "] " + message;

    // 输出到控制台
    if (level >= consoleLevel) {
        if (level >= LOG_WARNING) {
            std::cerr << logMessage << std::endl;
        } else {
            std::cout << logMessage << std::endl;
        }
    }

    // 输出到文件
    if (level >= fileLevel && logFile.is_open()) {
        logFile << logMessage << std::endl;
        logFile.flush();
    }
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
    case LOG_DEBUG:   return "DEBUG";
    case LOG_INFO:    return "INFO";
    case LOG_WARNING: return "WARNING";
    case LOG_ERROR:   return "ERROR";
    default:          return "UNKNOWN";
    }
}

std::string Logger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                            now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << milliseconds.count();
    return ss.str();
}

std::string Logger::getFileName(const std::string &fullPath) {
#ifdef _WIN32
    size_t lastSlash = fullPath.find_last_of("\\/");
#else
    size_t lastSlash = fullPath.find_last_of('/');
#endif
    if (lastSlash != std::string::npos) {
        return fullPath.substr(lastSlash + 1);
    }
    return fullPath;
}

void Logger::shutdown() {
    std::lock_guard<std::mutex> lock(logMutex);

    if (logFile.is_open()) {
        logFile << "\n========================================\n";
        logFile << "Application shutdown at: " << getCurrentTime() << "\n";
        logFile << "========================================\n";
        logFile.close();
    }
    initialized = false;
}
