#include "logsink.h"

void dblogger::StdOutSink::log(const char *data, size_t len)
{
    std::cout.write(data, len);
}

dblogger::RollingFileSink::RollingFileSink(const std::string &filename, const std::string& dir, size_t maxFileSize) 
: m_maxFileSize(maxFileSize), m_currentFileSize(0), m_nameCount(0), m_fileName(filename), m_dir(dir)
{
    std::filesystem::path dirPath(m_dir);
    if(!std::filesystem::exists(dirPath))
    {
        std::filesystem::create_directories(dirPath);
    }

    std::string fullPath = m_dir + "/" + m_fileName + "_" + Date::time2String(Date::now()) + std::to_string(m_nameCount) + ".log";
    m_ofs.open(fullPath, std::ios::binary | std::ios::app);
    if (!m_ofs.is_open())
    {
        throw std::runtime_error("Failed to open log file: " + fullPath);
    }
    
}

void dblogger::RollingFileSink::log(const char *data, size_t len)
{
    if(m_currentFileSize + len > m_maxFileSize)
    {
        m_ofs.close();
        m_nameCount++;
        m_currentFileSize = 0;
        std::string fullPath = m_dir + "/" + m_fileName + "_" + Date::time2String(Date::now()) + std::to_string(m_nameCount) + ".log";
        m_ofs.open(fullPath, std::ios::binary | std::ios::app);
        if (!m_ofs.is_open())
        {
            throw std::runtime_error("Failed to open log file: " + fullPath);
        }
    }
    m_ofs.write(data, len);
    m_currentFileSize += len;   
}
