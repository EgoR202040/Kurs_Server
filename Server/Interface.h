#pragma once
#include "Logger.h"
#include <string>
class Interface{
    int PORT;
public:
    Interface(int port=33333):PORT(port){}
    int comm_proc(int argc, char** argv);
};
