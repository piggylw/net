#pragma once
#include <string>
#include <memory>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "date.h"

namespace dblogger
{
/// @brief logsink用于设置日志输出的方式(标准输出，滚动文件)
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

/// @brief rolling file sink,日志滚动文件输出
class RollingFileSink : public LogSink
{
public:
    RollingFileSink(const std::string& filename, const std::string& dir, size_t maxFileSize = 100 * 1024 *1024);
    void log(const char* data, size_t len) override;

private:
    std::string m_fileName;
    std::string m_dir;
    size_t m_maxFileSize;
    size_t m_currentFileSize;
    size_t m_nameCount;
    std::ofstream m_ofs;
};

}