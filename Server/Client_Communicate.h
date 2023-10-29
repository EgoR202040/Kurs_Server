#pragma once
#include <string>
#include <vector>
#include <map>
#include "Logger.h"
#include "Errors.h"
using namespace std;
class Client_Communicate
{
    static int GetRandomNumber(int min, int max,int i);
    static std::string md5(std::string s);
    static std::string generate_salt();
    public:
    int connection(int port,std::map<std::string,std::string> database,Errors* err,Logger* l1);
};
