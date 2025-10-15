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

void dblogger::AsyncLooper::push(const char *data, size_t len)
{
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        if (m_stop) 
        {
            return;
        }
        //如果空间不够，先扩容，如果容量已经达到上限，则等待
        while(m_producerBuffer.writeAbleSize() < len && m_producerBuffer.size() < maxBufferSize)
            m_producerBuffer.resize(len);

        m_condProducer.wait(lock, [&](){
            return m_stop || m_producerBuffer.writeAbleSize() >= len;
        });

        if (m_stop) 
        {
            return;
        }
        m_producerBuffer.push(data, len);
    }
    m_condConsumer.notify_one();
}

void dblogger::AsyncLooper::threadFunc()
{
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            m_condConsumer.wait(lock, [&]() {
                return m_stop || m_producerBuffer.readAbleSize() > 0;
            });

            if(m_stop && m_producerBuffer.isEmpty())
                break;
            
            m_producerBuffer.swap(m_consumerBuffer);
        }
        m_condProducer.notify_all();
        m_cb(m_consumerBuffer);
        m_consumerBuffer.reset();
    } 
}

void dblogger::AsyncLooper::stop()
{
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_stop = true;
    }
    m_condProducer.notify_all();
    m_condConsumer.notify_all();
    if(m_td.joinable())
        m_td.join();
}

