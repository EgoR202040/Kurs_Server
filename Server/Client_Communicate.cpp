//Модули
#include "Client_Communicate.h"
#include "Logger.h"
#include "Errors.h"
#include "Calculator.h"

//Сетевое взаимодействие
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>

//Для хеширования
#include <sstream>
#include <cryptopp/hex.h>
#include <cryptopp/base64.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>

//Буст библиотеки для генерации SALT
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#define buff_size 1024

std::unique_ptr<char[]> buff(new char[buff_size]);
//************************************
//			Функция хеширования
//
//************************************
std::string Client_Communicate::md5(std::string s)
{
    using namespace CryptoPP;
    Weak::MD5 hash;
    std::string new_hash;
    StringSource(s, true,
                 new HashFilter(hash, new HexEncoder(new StringSink(new_hash))));
    return new_hash;
}
//************************************
//			Генерация SALT
//
//************************************
std::string Client_Communicate::generate_salt()
{
    time_t now = time(0);
    boost::mt19937 gen(now);
    boost::random::uniform_int_distribution<uint64_t> dist(0,std::numeric_limits<uint64_t>::max());
    boost::variate_generator<boost::mt19937&, boost::random::uniform_int_distribution<uint64_t>> getRand(gen, dist);
    std::stringstream s;
    s << std::hex << dist(gen);
    std::string results(s.str());
    while(results.length()<16) {
        results = '0' + results;
    }
    return results;
}
//************************************
//		  Функция соединения
//
//************************************
int Client_Communicate::connection(int port,std::map<std::string,std::string> database,Logger* l1)
{
    try {
        int queue_len = 100;
        sockaddr_in * addr = new (sockaddr_in);
        addr->sin_family = AF_INET;
        addr->sin_port = htons(port);
        inet_aton("127.0.0.1", &addr->sin_addr);
        int s = socket(AF_INET, SOCK_STREAM, 0); // TCP
        if (s==-1) {
            throw crit_err("Socket created err");
        } else {
            l1->writelog("listen socket created");
        }
        auto rc = bind(s,(const sockaddr*)addr,sizeof(sockaddr_in));
        if (rc==-1) {
            throw crit_err("Socket bind err");
        } else {
            l1->writelog("bind success");
        }
        rc = listen(s,queue_len);
        if(rc == -1) {
            throw crit_err("Socket listen err");
        }
        //************************************
        //		 Цикл обработки клиентов
        //
        //************************************
        for(;;) {
            try {
                sockaddr_in * client_addr = new sockaddr_in;
                socklen_t len = sizeof (sockaddr_in);
                int work_sock = accept(s, (sockaddr*)(client_addr), &len);
                if(work_sock == -1) {
                    throw no_crit_err("Client socket error");
                }
                l1->writelog("Client socket created");
                rc = recv(work_sock,buff.get(),buff_size,0);
                if(rc <= 0) {
                    close(work_sock);
                    throw no_crit_err("ID receive error");
                }
                l1->writelog("ID from client received");
                buff[rc]=0;
                std::string ID(buff.get(),rc);
                if(database.find(ID)==database.end()) {
                    close(work_sock);
                    throw no_crit_err("Unknown ID");
                }
                std::string salt_s = generate_salt();
                rc = send(work_sock,salt_s.c_str(),16,0);
                if(rc<=0) {
                    close(work_sock);
                    throw no_crit_err("send SALT error");
                }
                rc = recv(work_sock,buff.get(),buff_size,0);
                if(rc<=0) {
                    close(work_sock);
                    throw no_crit_err("HASH received error");
                }
                l1->writelog("HASH from client received");
                buff[rc]=0;
                std::string client_hash(buff.get(),rc);
                if(md5(salt_s+database[ID])!=client_hash) {
                    close(work_sock);
                    throw no_crit_err("Auth error");
                }
                rc = send(work_sock,"OK",2,0);
                if(rc<=0) {
                    close(work_sock);
                    throw no_crit_err("Send OK error");
                }
                int count;
                rc = recv(work_sock,&count,sizeof count,0);
                if(rc<=0) {
                    close(work_sock);
                    throw no_crit_err("Error: count of vectors not received");
                }
                l1->writelog("Vector count received");
                for(int i =0; i<count; i++) {
                    uint32_t vector_len;
                    rc = recv(work_sock,&vector_len,4,0);
                    if(rc<=0) {
                        close(work_sock);
                        throw no_crit_err("Error: len of vector not received");
                    }
                    std::unique_ptr<double[]> vector_data(new double[vector_len]);
                    rc = recv(work_sock,vector_data.get(),vector_len*sizeof(double),0);
                    if(rc<=0) {
                        close(work_sock);
                        throw no_crit_err("Error: vector not received");
                    }
                    std::vector<double> v(vector_data.get(),vector_data.get()+vector_len);
                    std::unique_ptr<Calculator[]> calc(new Calculator(v));
                    auto res = calc.get()->send_res();
                    rc = send(work_sock,&res,sizeof res,0);
                    if(rc==-1) {
                        close(work_sock);
                        throw no_crit_err("Error: result of calculating vector no send");
                    }
                    l1->writelog("Result of calculating vector send");
                }
            } catch(no_crit_err& e) {
                std::cerr << "Error with client: " << e.what() << std::endl;
                l1->writelog("Not critical error: " + *e.what());
            }
        }
    } catch(crit_err& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        l1->writelog("Critical error: " + *e.what());
    }
    return 0;
}
