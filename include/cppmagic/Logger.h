#pragma once

#include <cstdarg>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>

#include "cppmagic/MemoryAlias.h"

// Enum to represent log levels
enum LogLevel
{
    LOG_TRACE = 0,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

#define LOG_TRACE(message, ...) Logger::getInstance()->log(LogLevel::LOG_TRACE, __FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_DEBUG(message, ...) Logger::getInstance()->log(LogLevel::LOG_DEBUG, __FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_INFO(message, ...) Logger::getInstance()->log(LogLevel::LOG_INFO, __FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_WARN(message, ...) Logger::getInstance()->log(LogLevel::LOG_WARNING, __FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_ERROR(message, ...) Logger::getInstance()->log(LogLevel::LOG_ERROR, __FILE__, __LINE__, message, ##__VA_ARGS__)
#define LOG_CRITICAL(message, ...) Logger::getInstance()->log(LogLevel::LOG_CRITICAL, __FILE__, __LINE__, message, ##__VA_ARGS__)

class Logger
{
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static cmgSP<Logger> getInstance(const std::string& filename = "")
    {
        static cmgSP<Logger> instance(new Logger(filename));
        return instance;
    }

    void setLogLevel(LogLevel level)
    {
        auto logger = getInstance();
        std::scoped_lock<std::mutex> lock(logger->mLogMutex);
        logger->mCurrentLogLevel = level;
    }

    void setUseColor(bool useColor)
    {
        std::scoped_lock<std::mutex> lock(mLogMutex);
        mUseColor = useColor;
    }

    void log(LogLevel level, std::string codeFile, int lineNumber, const std::string& message)
    {
        auto logger = getInstance();
        std::scoped_lock<std::mutex> lock(logger->mLogMutex);

        if (level < logger->mCurrentLogLevel)
        {
            return;
        }

        std::ostringstream logEntry;
        std::string fileName = std::filesystem::path(codeFile).filename().string();
        logEntry << getTimestamp()
                 << " " << std::left << std::setw(20)
                 << (fileName + ":" + std::to_string(lineNumber))
                 << levelToString(level) << ": " << message;
        std::string logStr = logEntry.str() + "\n";

        if (logger->mUseColor)
            std::cerr << getColorForLevel(level) << logStr << RESET_COLOR;
        else
            std::cerr << logStr;

        if (logger->mLogFile.is_open())
        {
            logger->mLogFile << logStr;
            logger->mLogFile.flush();
        }
    }

    void log(LogLevel level, const std::string codeFile, int lineNumber, const char* fmt, ...)
    {
        auto logger = getInstance();
        std::scoped_lock<std::mutex> lock(logger->mLogMutex);

        if (level < logger->mCurrentLogLevel)
            return;

        char buffer[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);

        std::ostringstream logEntry;
        std::string fileName = std::filesystem::path(codeFile).filename().string();
        logEntry << getTimestamp()
                 << " " << std::left << std::setw(20)
                 << (fileName + ":" + std::to_string(lineNumber))
                 << levelToString(level) << ": " << buffer;
        std::string logStr = logEntry.str() + "\n";

        if (logger->mUseColor)
            std::cerr << getColorForLevel(level) << logStr << RESET_COLOR;
        else
            std::cerr << logStr;

        if (logger->mLogFile.is_open())
        {
            logger->mLogFile << logStr;
            logger->mLogFile.flush();
        }
    }

    ~Logger()
    {
        if (mLogFile.is_open())
        {
            mLogFile.close();
        }
    }

private:
    Logger(const std::string& filename)
    {
        if (!filename.empty())
        {
            mLogFile.open(filename, std::ios::app);
            if (!mLogFile.is_open())
            {
                std::cerr << "Error opening log file." << std::endl;
            }
        }
    }

    std::string levelToString(LogLevel level)
    {
        switch (level)
        {
        case LOG_TRACE:
            return "trace";
        case LOG_DEBUG:
            return "debug";
        case LOG_INFO:
            return "info";
        case LOG_WARNING:
            return "warning";
        case LOG_ERROR:
            return "error";
        case LOG_CRITICAL:
            return "critical";
        default:
            return "unknown";
        }
    }

    std::string getTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(now);
        auto localTime = std::localtime(&timeT);
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
        std::ostringstream oss;
        oss << std::put_time(localTime, "%H:%M:%S")
            << '.' << std::setw(6) << std::setfill('0') << microseconds.count();
        return oss.str();
    }

    std::string getColorForLevel(LogLevel level)
    {
        switch (level)
        {
        case LOG_TRACE:
            return LIGHT_GRAY_COLOR;
        case LOG_DEBUG:
            return CYAN_COLOR;
        case LOG_INFO:
            return GREEN_COLOR;
        case LOG_WARNING:
            return YELLOW_COLOR;
        case LOG_ERROR:
            return RED_COLOR;
        case LOG_CRITICAL:
            return MAGENTA_COLOR;
        default:
            return RESET_COLOR;
        }
    }

private:
    std::ofstream mLogFile;
    std::mutex mLogMutex;
    LogLevel mCurrentLogLevel = LOG_WARNING;
    bool mUseColor{true};

    const std::string RESET_COLOR = "\033[0m";
    const std::string RED_COLOR = "\033[31m";
    const std::string GREEN_COLOR = "\033[32m";
    const std::string YELLOW_COLOR = "\033[33m";
    const std::string BLUE_COLOR = "\033[34m";
    const std::string MAGENTA_COLOR = "\033[35m";
    const std::string CYAN_COLOR = "\033[36m";
    const std::string LIGHT_GRAY_COLOR = "\033[37m";
};
