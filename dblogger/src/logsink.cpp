#include "logsink.h"

void dblogger::StdOutSink::log(const char *data, size_t len)
{
    std::cout.write(data, len);
}