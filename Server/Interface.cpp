#include "Interface.h"
#include "Connector_to_base.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "Client_Communicate.h"
#include "Errors.h"
#include "Logger.h"
namespace po = boost::program_options;
int Interface::comm_proc(int argc, char** argv)
{
    bool flag_b = false;
    bool flag_l = false;
    bool flag_p = false;
    std::string logfile;
    std::string basefile;
    //************************************
    //			Разбор ПКС
   	//
   	//************************************
    try {
        po::options_description opts("Allowed options");
        opts.add_options()
        ("help,h", "Show help")
        ("basefile,b",
         po::value<std::string>()->default_value("base.txt"),
         "option is string(path to file with database)") 
        ("logfile,l",                        
         po::value<std::string>()->default_value("log.txt"), 
         "option is string(path to file with logs)")
        ("PORT,p",                               
         po::value<int>()->default_value(PORT),
         "option is int (PORT for server)");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, opts), vm);
        po::notify(vm);
        if(vm.count("help")) {
            std::cout << opts << "\n";
            exit(0);
        }

        if(vm.count("basefile")) {
            if(vm["basefile"].as<std::string>() == "base.txt") {
                flag_b = true;
            }
            basefile = vm["basefile"].as<std::string>();
        }
        if(vm.count("logfile")) {
            if(vm["logfile"].as<std::string>() == "log.txt") {
                flag_l = true;
            }
            logfile = vm["logfile"].as<std::string>();
        }

        if(vm.count("PORT")) {
            if(vm["PORT"].as<int>() == 33333) {
                flag_p = true;
            }
            PORT = vm["PORT"].as<int>();
        }

        if(flag_b and flag_l and flag_p) {
            std::cout << "Server started with default parameters.Use -h for help"<<std::endl;
        }}catch(po::error& e) {
        std::cerr << "error: " << e.what() << "\n";
        std::cerr << "Use -h for help\n";
        return 1;
    }catch(std::invalid_argument &err){
    	std::cerr <<"error: " << err.what() << "\n";
    	std::cerr << "Use -h for help\n";
    	return 1;
    } catch(std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        std::cerr << "Use -h for help\n";
        return 1;
    } catch(...) {
        std::cerr << "Exception of unknown type!\n";
        std::cerr << "Use -h for help\n";
        std::terminate();
    }
    //************************************
    //	   Создание объектов классов
   	//
   	//************************************
    Errors err;
    Logger debuges;
    if(debuges.set_path(logfile)==1) {
        std::cerr<<"Path not found for logfile" << std::endl;
        throw std::invalid_argument("path to logfile incorrect");
    }
    Logger l1(logfile);
    if(logfile[0] == '/' or logfile=="log.txt") {
        if(logfile != "/home/stud/log.txt") {
            l1.writelog("Path to logfile set value: "+logfile);
        } else {
            l1.writelog("Path to logfile set default value");
        }
    } else {
        throw std::invalid_argument("path to logfile incorrect");
    }


    if(basefile != "/home/stud/base.txt") {
        l1.writelog("Path to basefile set value: " + basefile);
    } else {
        l1.writelog("Path to basefile set default value");
    }
    if(PORT != 33333) {
        l1.writelog("Port set not default value");
    } else {
        l1.writelog("Port set default value");
    }
    Connector_to_base c1;
    if(c1.connect_to_base(basefile)==1) {
        l1.writelog("Base not found");
        throw std::invalid_argument("path to basefile");
    } else {
        l1.writelog("Connect to database success!");
    }
    l1.writelog("Server started");
    Client_Communicate con;
    if(con.connection(PORT,c1.get_data(),&err,&l1)==1) {
        std::cerr << "Errow with communicate with client" << std::endl;
        return 1;
    };
    return 0;
}
