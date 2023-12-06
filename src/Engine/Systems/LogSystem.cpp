#include "LogSystem.h"

#include "EngineSettings.h"

#include <chrono>
#include <ctime>
#include <cstdarg>
#include <cstdio>
#include <iomanip>
#include <iostream>

LogSystem gLogger;

LogSystem::LogSystem()
{
    _fileHandle.open(ENGINE_LOG_PATH, std::ios::trunc);
    _fileHandle.fill('0');
    std::cout.fill('0');

    auto now = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(now);
    auto pTime = std::localtime(&tt);

    _fileHandle << "EngineLog -> " << pTime->tm_year + 1900 << '/' << pTime->tm_mon + 1 << '/' << pTime->tm_mday << std::endl;
}

LogSystem::~LogSystem()
{
    if (_fileHandle.is_open())
        _fileHandle.close();
}

void LogSystem::log(LogLevel level, const char *position, const char *fmt, ...)
{
    static char buffer[256]; // POTENTIAL RISK OF OVERFLOW...
    _logBuf.str("");
    _logBuf.clear();

    switch (level)
    {
    case LogLevel::INFO:
        _logBuf << "[INFO / ";
        break;
    case LogLevel::WARNING:
        _logBuf << "[WARNING / ";
        break;
    case LogLevel::ERROR:
        _logBuf << "[ERROR / ";
        break;
    default:
        break;
    }

    printTime();

    _logBuf << "] <" << position << "> ";

    std::va_list args;
    va_start(args, fmt);
    std::sprintf(buffer, fmt, args);
    va_end(args);

    _logBuf << buffer << '\n';

    std::string_view logMsg = _logBuf.str();
    _fileHandle << logMsg << std::flush;
    std::cout << logMsg << std::flush;
}

void LogSystem::printTime()
{
    auto now = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(now);
    auto pTime = std::localtime(&tt);

    _logBuf << std::setw(2) << pTime->tm_hour << ':' << std::setw(2) << pTime->tm_min << ':' << std::setw(2) << pTime->tm_sec;
}
