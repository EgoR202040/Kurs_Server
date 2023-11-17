#include <iostream>
#include <fstream>
#include "Errors.h"
#include "Logger.h"
#include "Connector_to_base.h"
using namespace std;
int Connector_to_base::connect_to_base(string base_file)
{
    ifstream file_read;
    file_read.open(base_file);
    string line;
    string temp_pass;
    string temp_login;
    if (file_read.is_open()) {
        while(getline(file_read,line)) {
            temp_login = "";
            temp_pass = "";
            bool flag = true;
            for(char s:line) {
                if(s == ':') {
                    flag = false;
                }
                if(flag) {
                    temp_login.push_back(s);
                } else {
                    temp_pass.push_back(s);
                }
            }
            temp_pass.erase(0,1);
            data_base[temp_login]= temp_pass;
        }
        return 0;
    } else {
        throw crit_err("invalid_base_path");
    }
};
