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
    int PORT = 0;
    try {
        // создание пустой коллекции ПКС
        po::options_description opts("Allowed options"); // параметр - заголовок справки по коллекции ПКС
        // добавление параметров в коллекцию ПКС
        opts.add_options()
        ("help,h", "Show help") // парметр-переключатель - имя (длинное и короткое) и текст справки
        //дальше все параметры со значениями
        // параметр --first
        ("basefile,b", // имя (длинное)
         po::value<std::string>()->default_value("base.txt"), // значение типа std::string, сохраняется дополнительно в переменную s
         "option is string(path to file with database)")   // текст справки
        // параметр --second, -s
        ("logfile,l",                          // имя (длинное и короткое)
         po::value<std::string>()->default_value("log.txt"), //значение типа int, при умолчании = 25
         "option is string(path to file with logs)") // текст справки
        // параметр --third, -t
        ("PORT,p",                                // имя (длинное и короткое)
         po::value<int>()->default_value(33333), //значение типа float, при пропуске = 3.14
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
        }

    } catch(po::error& e) {
        std::cerr << "error: " << e.what() << "\n";
        std::cerr << "Use -h for help\n";
        return 1;
    } catch(std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    } catch(...) {
        std::cerr << "Exception of unknown type!\n";
        std::terminate();
    }
    Errors err;
    Logger debuges;
    if(debuges.set_path(logfile)==1) {
        std::cerr<<"Path not found for logfile" << std::endl;
        std::terminate();
    }
    Logger l1(logfile);
    if(logfile[0] == '/' or logfile=="log.txt") {
        if(logfile != "/home/stud/log.txt") {
            l1.writelog("Path to logfile set value: "+logfile);
        } else {
            l1.writelog("Path to logfile set default value");
        }
    } else {
        std::cerr << "Incorrect path to log" << std::endl;
        return 1;
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
        err.error_processing(1,&l1);
        l1.writelog("Base not found");
        return 1;
    } else {
        l1.writelog("Connect to database succes!");
    }
    l1.writelog("Server started");
    Client_Communicate con;
    if(con.connection(PORT,c1.get_data(),&err,&l1)==1) {
        std::cerr << "Troubles with server" << std::endl;
        return 1;
    };
    return 0;
}
