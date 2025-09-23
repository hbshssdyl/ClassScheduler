#include "Logger.h"
#include "Utils/DataUtils.h"
#include <filesystem>
#include <algorithm>
#include <cstdio>   // for std::rename, std::remove
#include <sys/stat.h> // for stat()
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define mkdir _mkdir
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

namespace fs = std::filesystem;

bool fileExists(const std::string &path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : consoleLevel(LOG_DEBUG), fileLevel(LOG_DEBUG), initialized(false) {
}

Logger::~Logger() {
    shutdown();
}
fs::path logPath;
void Logger::init(const std::string &filename, LogLevel consoleLevel, LogLevel fileLevel) {
    {
        std::lock_guard<std::mutex> lock(logMutex);
        if (initialized) {
            return;
        }
        this->consoleLevel = consoleLevel;
        this->fileLevel = fileLevel;

        // 使用 %APPDATA% 作为日志基础路径
        std::string pathDir = ClassScheduler::currentPathDir().toStdString();
        const char* appDataEnv = pathDir.c_str();
        if (!appDataEnv) {
            appDataEnv = std::getenv("LOCALAPPDATA"); // 备用
        }
        if (!appDataEnv) {
            std::cerr << "无法获取用户应用数据目录，使用当前目录。" << std::endl;
            logPath = filename; // 使用类成员 logPath
        } else {
            fs::path baseDir(appDataEnv);
            fs::path appDir = baseDir / "logs"; // 自定义主目录
            fs::path fullDir = appDir / fs::path(filename).parent_path(); // 包括 logs 子目录
            logPath = fullDir / fs::path(filename).filename(); // 完整路径包括文件名

            // 创建完整目录结构
            std::error_code ec;
            fs::create_directories(fullDir, ec);
            if (ec) {
                std::cerr << "创建日志目录失败: " << ec.message() << std::endl;
                return;
            }

            // 构造 last_ 文件名
            fs::path lastFile = fullDir / ("last_" + fs::path(filename).filename().string());
            if (fs::exists(logPath)) {
                if (fs::exists(lastFile)) {
                    fs::remove(lastFile, ec);
                    if (ec) {
                        std::cerr << "删除旧 last log 失败: " << ec.message() << std::endl;
                    }
                }
                fs::rename(logPath, lastFile, ec);
                if (ec) {
                    std::cerr << "重命名旧日志文件失败: " << ec.message() << std::endl;
                }
            }

            // 创建新日志文件
            logFile.open(logPath, std::ios::out | std::ios::trunc);
            if (logFile.is_open()) {
                logFile << "========================================\n";
                logFile << "应用程序启动时间: " << getCurrentTime() << "\n";
                logFile << "========================================\n\n";
                initialized = true;
            } else {
                std::cerr << "无法打开日志文件: " << logPath << std::endl;
                return;
            }
        }
    }
    LOG_INFO("日志系统初始化成功");
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
