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
    void print_result();
    int connection(int port,std::map<std::string,std::string> database,Errors* err,Logger* l1);
    int auth_process(char ID,std::map<std::string,std::string> database);
    void disconnect();
    std::vector<std::vector<uint32_t>> receive_data();
};
