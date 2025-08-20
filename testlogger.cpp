#include "logger.h"
#include <iostream>

int main()
{
    dblogger::Formatter::ptr formatter = std::make_unique<dblogger::Formatter>();
    dblogger::LogSink::ptr stdOutSink= std::make_unique<dblogger::StdOutSink>();
    std::vector<dblogger::LogSink::ptr> vec;
    vec.push_back(std::move(stdOutSink));

    dblogger::SyncLogger logger("sync", dblogger::LogLevel::Level::DEBUGE, std::move(formatter), std::move(vec));
    logger.info(__FILE__, __FUNCTION__, __LINE__, "nihao");
}