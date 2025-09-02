#include "logger.h"
#include <iostream>

int main()
{
    dblogger::Formatter::ptr formatter = std::make_unique<dblogger::Formatter>();
    dblogger::LogSink::ptr stdOutSink= std::make_unique<dblogger::StdOutSink>();
    dblogger::LogSink::ptr rollingFileSink= std::make_unique<dblogger::RollingFileSink>("testlog", "./logs", 20);
    std::vector<dblogger::LogSink::ptr> vec;
    vec.push_back(std::move(stdOutSink));
    vec.push_back(std::move(rollingFileSink));

    dblogger::SyncLogger logger("sync", dblogger::LogLevel::Level::DEBUGE, std::move(formatter), std::move(vec));

    logger.debug(__FILE__, __FUNCTION__, __LINE__, "This is a debug message");
    logger.info(__FILE__, __FUNCTION__, __LINE__, "This is an info message");
    logger.warning(__FILE__, __FUNCTION__, __LINE__, "This is a warning message");
    logger.error(__FILE__, __FUNCTION__, __LINE__, "This is an error message");
    logger.fatal(__FILE__, __FUNCTION__, __LINE__, "This is a fatal message");
    
}