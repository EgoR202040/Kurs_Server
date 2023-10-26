#pragma once
#include "Logger.h"
#include <string>
class Interface{
public:
friend int Logger::writelog(std::string s);
    int PORT;
    std::string IP;
    int ID;
    Interface(){PORT = 33333;IP = "none";ID = 13;};
    int comm_proc(int argc, char** argv);
};
