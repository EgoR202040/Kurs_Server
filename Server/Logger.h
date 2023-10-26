#pragma once
#include <string>
class Logger
{
    static std::string getCurrentDateTime(std::string s);
    std::string path_to_logfile;
public:
    void writelog(std::string s);
    Logger(std::string s){path_to_logfile = s;};
};
