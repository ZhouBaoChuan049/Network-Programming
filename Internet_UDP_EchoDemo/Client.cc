#include "Log.hpp"
#include "Mutex.hpp"
#include <sys/socket.h> 
#include <netinet/in.h>  
#include <sys/types.h>
#include <functional>
#include <cstdio>
#include <cstring>
#include <string>
#include <arpa/inet.h>
using namespace LogModule ;
int main(int argc , char* args [])
{
    if(argc != 3)
    {
        std::cout<<"你输入的命令行参数有错误，应该是三个！"<<std::endl;
        return 1;
    }
    int socketfd= socket(AF_INET , SOCK_DGRAM ,0);
    if(socketfd < 0)
    {
        std::cerr << "socket error" << std::endl;
        return 2;
    }
    struct sockaddr_in server ;
    memset(&server , 0 , sizeof(server));
    server.sin_family = AF_INET ;
    server.sin_port = htons(std::stoi(args[2]));
    server.sin_addr.s_addr = inet_addr(args[1]);
    while(true)
    {
        std::string buffer;
        std::cout<<"Please Enter#";
        std::getline(std::cin , buffer);
        socklen_t len = sizeof(server);
        int n = sendto(socketfd ,buffer.c_str() , buffer.size(),0,(struct sockaddr*)&server , len);
        (void)n;
        char _buffer[1024] ;
        struct sockaddr_in peer ;
        memset(&peer , 0 , sizeof(peer));
        socklen_t _len = sizeof(peer) ;
        int m = recvfrom(socketfd ,_buffer ,sizeof(_buffer)-1,0,(struct sockaddr*)&peer,&_len);
        if(m > 0)
        {
            _buffer[m] =0 ;
            std::cout<<_buffer<<std::endl;
        }
    }
    return 0 ;
}