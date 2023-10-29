#include "Errors.h"
#include <iostream>
#include <string>
#include "Logger.h"
void Errors::error_processing(int err,Logger* l)
{
    if(err!=0) {
        if(err == 1) {
            std::cerr << "Connection to basefile error: file not found" << std::endl;
            l->writelog("Connection to basefile error: file not found");
        }
        if(err == 2) {
            std::cerr << "Error with connection" << std::endl;
            l->writelog("Error with connection");
        }
        if(err == 3) {
            std::cerr << "Error with listen socket" << std::endl;
            l->writelog("Fatal Error: Error with listen socket created");
        }
        if(err == 4) {
            std::cerr << "Error with bind socket to IP" << std::endl;
            l->writelog("Fatal Error: Error with bind");
        }
        if(err == 5) {
            std::cerr << "Listen function fail" << std::endl;
            l->writelog("Fatal Error: Error with listen funct");
        }
        if(err == 6) {
            std::cerr << "Error with communicate socket " << std::endl;
            l->writelog("Error with communicate socket");
        }
        if(err == 7) {
            std::cerr << "Error with recv" << std::endl;
            l->writelog("Error with recv function");
        }
        if(err == 8) {
            std::cerr << "Error: unknown ID" << std::endl;
            l->writelog("Unknown ID received");
        }
        if(err == 9) {
            std::cerr << "Error: send result of calculation" << std::endl;
            l->writelog("Error with send data");
        }
        if(err == 10) {
            std::cerr << "Authentication error" << std::endl;
            l->writelog("Error with Authentication");
        }
    }
}
