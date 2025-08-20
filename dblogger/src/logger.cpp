#include "logger.h"

void dblogger::Logger::debug(const std::string& fileName, const std::string& functionName, size_t line, const std::string& other)
{
    if(LogLevel::Level::DEBUGE < m_limitLevel)
        return;

    LogMsg msg(LogLevel::Level::DEBUGE, m_loggerName, fileName, functionName, line, other);
    std::string logMessage = m_formatter->format(msg);
    log(logMessage.c_str(), logMessage.size());
}

void dblogger::Logger::info(const std::string& fileName, const std::string& functionName, size_t line, const std::string& other)
{
    if(LogLevel::Level::INFO < m_limitLevel)
        return;

    LogMsg msg(LogLevel::Level::INFO, m_loggerName, fileName, functionName, line, other);
    std::string logMessage = m_formatter->format(msg);
    log(logMessage.c_str(), logMessage.size());
}

void dblogger::Logger::warning(const std::string& fileName, const std::string& functionName, size_t line, const std::string& other)
{
    if(LogLevel::Level::WARNNING < m_limitLevel)
        return;

    LogMsg msg(LogLevel::Level::WARNNING, m_loggerName, fileName, functionName, line, other);
    std::string logMessage = m_formatter->format(msg);
    log(logMessage.c_str(), logMessage.size());
}

void dblogger::Logger::error(const std::string& fileName, const std::string& functionName, size_t line, const std::string& other)
{
    if(LogLevel::Level::ERROR < m_limitLevel)
        return;

    LogMsg msg(LogLevel::Level::ERROR, m_loggerName, fileName, functionName, line, other);
    std::string logMessage = m_formatter->format(msg);
    log(logMessage.c_str(), logMessage.size());
}

void dblogger::Logger::fatal(const std::string& fileName, const std::string& functionName, size_t line, const std::string& other)
{
    if(LogLevel::Level::FATAL < m_limitLevel)
        return;

    LogMsg msg(LogLevel::Level::FATAL, m_loggerName, fileName, functionName, line, other);
    std::string logMessage = m_formatter->format(msg);
    log(logMessage.c_str(), logMessage.size());
}

void dblogger::SyncLogger::log(const char *data, size_t len)
{
    std::unique_lock<std::mutex> lock(m_mtx);
    for(auto& sink : m_logsink)
    {
        if(sink.get() != nullptr)
            sink->log(data, len);
    }
}
