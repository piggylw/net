#pragma once
#include "loglevel.h"
#include "date.h"
#include <ctime>
#include <string>
#include <thread>

namespace dblogger
{
/// @brief 日志消息类，包含了日志的所有信息
struct LogMsg
{
    time_t m_time;
    LogLevel::Level m_level;
    std::string m_loggerName;
    std::thread::id m_tid;
    std::string m_fileName;
    std::string m_functionName;
    size_t m_line;
    std::string m_other;

    LogMsg(
        LogLevel::Level level, 
        const std::string& loggerName,
        const std::string& fileName,
        const std::string& functionName,
        size_t line,
        const std::string& other
    ):  m_time(Date::now()),
        m_level(level),
        m_loggerName(std::move(loggerName)),
        m_tid(std::this_thread::get_id()),
        m_fileName(std::move(fileName)),
        m_functionName(std::move(functionName)),
        m_line(line),
        m_other(other)
        {}
};


}