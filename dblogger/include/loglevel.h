#pragma once
#include <string>

namespace dblogger
{

class LogLevel
{
public:
    enum class Level
    {
        UNKONWN = 0,
        DEBUGE,
        INFO,
        WARNNING,
        ERROR,
        FATAL,
        OFF
    };

    static std::string toString(LogLevel::Level level)
    {
        switch (level)
        {
        case LogLevel::Level::DEBUGE: return "DEBUGE";
        case LogLevel::Level::INFO: return "INFO";
        case LogLevel::Level::WARNNING: return "WARNNING";
        case LogLevel::Level::ERROR: return "ERROR";
        case LogLevel::Level::FATAL: return "FATAL";
        case LogLevel::Level::OFF: return "OFF";
        default:
            break;
        }
        return "UNKONWN";
    }
};

}