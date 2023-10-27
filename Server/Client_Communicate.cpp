#include "Client_Communicate.h"
#include <netinet/in.h>
#include "Logger.h"
#include "Errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <cryptopp/hex.h>
#include <cryptopp/base64.h>
#include "Calculator.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
char buff[1024];
std::string Client_Communicate::md5(std::string s)
{
    using namespace CryptoPP;
    Weak::MD5 hash;
    std::string new_hash;
    //Цепочка преобразований
    StringSource(s, true,
                 new HashFilter(hash, new HexEncoder(new StringSink(new_hash))));
    return new_hash;
}
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
/*
int Client_Communicate::GetRandomNumber(int min, int max,int i)
{
    srand(time(NULL)+i);
    int num = min + rand() % (max - min + 1);
    return num;
}

std::string Client_Communicate::generate_salt()
{
    static std::string alph = {
        "0123456789ABCDEFabcdef"
    };
    std::string Salt;
    for(int i = 0 ; i < 16; i++)
        Salt +=alph.at(GetRandomNumber(0,15,i));
    return Salt;
}*/
int Client_Communicate::connection(int port,std::map<std::string,std::string> database,Errors* err,Logger* l1)
{
    int queue_len = 100;
    sockaddr_in * addr = new (sockaddr_in);
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    inet_aton("127.0.0.1", &addr->sin_addr);
    //addr->sin_addr.s_addr = inet_addr("127.0.0.1"); - старый метод
    int s = socket(AF_INET, SOCK_STREAM, 0); // TCP
    if (s==-1) {
        err->error_processing(3,l1);
        return 1;
    } else {
        l1->writelog("listen socket created");
    }
    auto rc = bind(s,(const sockaddr*)addr,sizeof(sockaddr_in));
    if (rc==-1) {
        err->error_processing(4,l1);
        return 1;
    } else {
        l1->writelog("bind success");
    }
    rc = listen(s,queue_len);
    if(rc == -1) {
        err->error_processing(5,l1);
        return 1;
    }
    for(;;) {
        sockaddr_in * client_addr = new sockaddr_in;
        socklen_t len = sizeof (sockaddr_in);
        int work_sock = accept(s, (sockaddr*)(client_addr), &len);
        if (work_sock == -1) {
            err->error_processing(6,l1);
        } else {
            l1->writelog("Client socket created");
            rc = recv(work_sock,&buff,sizeof buff,0);
            if(rc == -1) {
                err->error_processing(7,l1);
            } else {
                l1->writelog("ID from client received");
                std::string ID(buff,rc);
                memset(&buff,0,sizeof buff);
                if(database.find(ID) != database.end()) {
                    std::string salt_s = generate_salt();
                    send(work_sock,salt_s.c_str(),16,0);
                    rc = recv(work_sock,&buff,sizeof buff,0);
                    if(rc==-1) {
                        err->error_processing(7,l1);
                    } else {
                    	l1->writelog("hash from client received");
                        std::string client_hash(buff,rc);
                        memset(&buff,0,sizeof buff);
                        if(md5(salt_s+database[ID])==client_hash) {
                            rc = send(work_sock,"OK",2,0);
                            int count;
                            rc = recv(work_sock,&count,sizeof count,0);
                            if(rc==-1) {
                                err->error_processing(7,l1);
                            } else {
                            	l1->writelog("Count vectors from client received");
                                Calculator calc;
                                for(int i = 0; i<count; i++) {
                                    uint32_t vector_len;
                                    rc = recv(work_sock,&vector_len,4,0);
                                    if(rc==-1) {
                                        err->error_processing(7,l1);
                                        close(work_sock);
                                        break;
                                    } else {
                                        std::unique_ptr<double[]> vector_data(new double[vector_len]);
                                        rc = recv(work_sock,vector_data.get(),vector_len * sizeof(double),0);
                                        if(rc == -1) {
                                            err->error_processing(7,l1);
                                            close(work_sock);
                                            break;
                                        } else {
                                            std::vector<double> v(vector_data.get(),vector_data.get()+vector_len);
                                            auto res = calc.calculate(v);
                                            rc = send(work_sock,&res,sizeof res,0);
                                            if(rc==-1){
                                            err->error_processing(9,l1);
                                            close(work_sock);
                                            }else{l1->writelog("send result of calculation");}
                                        }
                                    }
                                }
                            }
                        } else {
                            send(work_sock,"ERR",3,0);
                            err->error_processing(7,l1);
                            close(work_sock);
                        }
                    }
                } else {
                    send(work_sock,"ERR",3,0);
                    err->error_processing(7,l1);
                    close(work_sock);
                }
            }
        }
        //close(work_sock);
    }
    return 0;
}
