#pragma once
#include "logmsg.h"
#include "date.h"
#include <memory>
#include <sstream>

namespace dblogger
{
/// @brief Formatter用于设置日志的格式，此处固定格式，不支持拓展
class Formatter
{
public:
    using ptr = std::unique_ptr<Formatter>;
    void format(std::ostream& out, const LogMsg& msg);
    std::string format(const LogMsg& msg);

};

}
