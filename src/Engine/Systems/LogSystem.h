#pragma once
#include <fstream>
#include <sstream>

enum class LogLevel
{
    INFO,
    WARNING,
    ERROR
};

class ConInterface;

class LogSystem
{
public:
    LogSystem();
    ~LogSystem();

    void log(LogLevel level, const char *position, const char *fmt, ...);

private:
    void printTime();

private:
    std::ofstream _fileHandle;
    std::stringstream _logBuf;
    ConInterface *_conInterface;
};

extern LogSystem gLogger;

#define LOG_INFO(...) gLogger.log(LogLevel::INFO, __FUNCTION__, __VA_ARGS__)
#define LOG_WARNING(...)                                                       \
    gLogger.log(LogLevel::WARNING, __FUNCTION__, __VA_ARGS__)
#define LOG_ERROR(...) gLogger.log(LogLevel::ERROR, __FUNCTION__, __VA_ARGS__)
