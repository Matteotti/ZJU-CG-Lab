#include "LogSystem.h"

#include "EngineSettings.h"
#include "Platform/ConInterface.h"

#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <iostream>

LogSystem gLogger;

LogSystem::LogSystem()
{
    _conInterface = ConInterface::getConInterface();

    _logBuf.fill('0');

    _fileHandle.open(ENGINE_LOG_PATH, std::ios::trunc);

    auto now = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(now);
    auto pTime = std::localtime(&tt);

    _fileHandle << "EngineLog -> " << pTime->tm_year + 1900 << '/' << pTime->tm_mon + 1 << '/' << pTime->tm_mday
                << std::endl;
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

    switch (level)
    {
    case LogLevel::INFO:
        _conInterface->SetTextColor(11);
        _logBuf << "[INFO / ";
        break;
    case LogLevel::WARNING:
        _conInterface->SetTextColor(14);
        _logBuf << "[WARNING / ";
        break;
    case LogLevel::ERROR:
        _conInterface->SetTextColor(12);
        _logBuf << "[ERROR / ";
        break;
    default:
        break;
    }

    printTime();

    _logBuf << "] <" << position << "> ";

    std::va_list args;
    va_start(args, fmt);
    std::vsprintf(buffer, fmt, args);
    va_end(args);

    _logBuf << buffer << '\n';

    std::string logMsg = _logBuf.str();
    _fileHandle << logMsg << std::flush;
    std::cout << logMsg << std::flush;
}

void LogSystem::printTime()
{
    auto now = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(now);
    auto pTime = std::localtime(&tt);

    _logBuf << std::setw(2) << pTime->tm_hour << ':' << std::setw(2) << pTime->tm_min << ':' << std::setw(2)
            << pTime->tm_sec;
}
