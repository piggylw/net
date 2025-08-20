#pragma once
#include <iostream>
#include <ctime>

namespace dblogger
{

class Date
{
public:
    static time_t now()
    {
        return time(nullptr);
    }

    static std::string time2String(time_t nowTime)
    {
        char buf[64] = {0};
        struct tm *sysTime = localtime(&nowTime);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", sysTime);
        return buf;
    }
};

}