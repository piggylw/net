#pragma once
#include "logsink.h"
#include "formatter.h"
#include "buffer.h"
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <functional>

namespace dblogger
{
/// @brief Logger是日志的核心类，包含了日志名，日志权限，日志格式化器，日志输出接口等
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

/// @brief 同步日志器
class SyncLogger : public Logger
{
public:
    explicit SyncLogger(const std::string& loggerName, LogLevel::Level limitLevel, Formatter::ptr formatter,
        std::vector<LogSink::ptr> logsink) : Logger(loggerName, limitLevel, std::move(formatter), std::move(logsink))
    {}

protected:
    void log(const char* data, size_t len) override;
};

/// @brief 异步日志器
class AsyncLooper
{
public:
    using callback = std::function<void(Buffer&)>;

    explicit AsyncLooper(callback cb, size_t maxBufferSize = 10 * 1024 * 1024)
        : maxBufferSize(maxBufferSize), m_stop(false), m_cb(cb)
    {
        m_td = std::thread(&AsyncLooper::threadFunc, this);
    }

    ~AsyncLooper()
    {
        stop();
    }

    void push(const char* data, size_t len);

private:
    void threadFunc();
    void stop();

private:
    size_t maxBufferSize;
    std::thread m_td;
    Buffer m_producerBuffer;
    Buffer m_consumerBuffer;
    std::mutex m_mtx;
    std::condition_variable m_condProducer;
    std::condition_variable m_condConsumer;
    std::atomic<bool> m_stop;
    callback m_cb;
};

class AsyncLogger : public Logger
{
public:
    explicit AsyncLogger(const std::string& loggerName, LogLevel::Level limitLevel, Formatter::ptr formatter,
        std::vector<LogSink::ptr> logsink, size_t maxBufferSize= 10 * 1024 * 1024) 
        : Logger(loggerName, limitLevel, std::move(formatter), std::move(logsink)),
        m_looper(std::bind(&AsyncLogger::realSink, this, std::placeholders::_1), maxBufferSize)
    {}

protected:
    void log(const char* data, size_t len) override
    {
        m_looper.push(data, len);
    }

private:
    void realSink(Buffer& buf)
    {
        for(auto& sink : m_logsink)
        {
            if(sink.get() != nullptr)
                sink->log(buf.start(), buf.readAbleSize());
        }
    }

private:
    AsyncLooper m_looper;
};
   
}