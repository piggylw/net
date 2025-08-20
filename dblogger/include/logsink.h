#pragma once
#include <string>
#include <memory>
#include <iostream>

namespace dblogger
{

//logsink用于设置日志输出的方式(标准输出，滚动文件)
class LogSink
{
public:
    using ptr = std::unique_ptr<LogSink>;
    // 日志输出接口, data为日志的其实地址,len为日志的长度
    virtual void log(const char* data, size_t len) = 0;
    virtual ~LogSink() {};
};

class StdOutSink : public LogSink
{
public:
    void log(const char* data, size_t len) override;
};

}