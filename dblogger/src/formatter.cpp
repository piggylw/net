#include "formatter.h"

void dblogger::Formatter::format(std::ostream &out, const LogMsg &msg)
{
    // [time] [level] [name] [tid] [filename : functionname : line] othermsg
    out << "[" << Date::time2String(msg.m_time) << "]";
    out << "[" << LogLevel::toString(msg.m_level) << "]";
    out << "[" << msg.m_loggerName << "]";
    out << "[" << msg.m_tid << "]";
    out << "[" << msg.m_fileName << " : " << msg.m_functionName << " : " << msg.m_line <<"]";
    out << " " << msg.m_other << "\n";
}

std::string dblogger::Formatter::format(const LogMsg &msg)
{
    std::ostringstream ss;
    format(ss, msg);
    return ss.str();
}
