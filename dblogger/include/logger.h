#pragma once
#include "logsink.h"
#include "formatter.h"
#include <vector>
#include <mutex>

namespace dblogger
{

class Logger
{
public:
    using ptr = std::unique_ptr<Logger>;
    explicit Logger(const std::string& loggerName, LogLevel::Level limitLevel, Formatter::ptr formatter,
        std::vector<LogSink::ptr> logsink)
        : m_loggerName(loggerName),
          m_limitLevel(limitLevel),
          m_formatter(std::move(formatter)),
          m_logsink(std::move(logsink))
          {}

    void debug(const std::string& fileName, const std::string& functionName, size_t line, const std::string& other);
    void info(const std::string& fileName, const std::string& functionName, size_t line, const std::string& other);
    void warning(const std::string& fileName, const std::string& functionName, size_t line, const std::string& other);
    void error(const std::string& fileName, const std::string& functionName, size_t line, const std::string& other);
    void fatal(const std::string& fileName, const std::string& functionName, size_t line, const std::string& other);

protected:
	// 通过log接口让不同的日志器支持同步落地或者异步落地
	virtual void log(const char* data, size_t len) = 0;

protected:
    std::string m_loggerName;
    LogLevel::Level m_limitLevel;
    std::mutex m_mtx;
    Formatter::ptr m_formatter;
    std::vector<LogSink::ptr> m_logsink;
};


class SyncLogger : public Logger
{
public:
    explicit SyncLogger(const std::string& loggerName, LogLevel::Level limitLevel, Formatter::ptr formatter,
        std::vector<LogSink::ptr> logsink) : Logger(loggerName, limitLevel, std::move(formatter), std::move(logsink))
    {}

protected:
    void log(const char* data, size_t len) override;
};

}