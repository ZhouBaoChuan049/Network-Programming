#ifndef __INETADDR__
#define __INETADDR__
#include "Log.hpp"
#include "Common.hpp"
using namespace LogModule ;
namespace InetAddrModule
{
    class InetAddr
    {
    public:
        InetAddr(sockaddr_in addr)
        {
            //网络转本地
            _Port = ntohs(addr.sin_port) ;
            char tmp [1024] ;
            if(!inet_ntop(AF_INET , &(addr.sin_addr) , 
            tmp, sizeof(tmp)))
                LOG(LEVEL::ERROR)<<"网络序列转本机序列错误"<<CAGE;
            else _LocalIp = tmp ;
        }
        InetAddr(const char* ip , char* port)
        {
            //本地转网络-1
            _Port = std::stoi(port);
            _LocalIp = ip ;
            memset(&_Addr , 0 , sizeof(_Addr));
            _Addr.sin_family = AF_INET ;
            _Addr.sin_port = htons(_Port);
            if(inet_pton(AF_INET ,_LocalIp.c_str(), &_Addr.sin_addr) <= 0)
                LOG(LEVEL::ERROR)<<"本机序列转网络序列错误"<<CAGE;
        }
        InetAddr(const char* port)
        {
            //本地转网络-2
            _Port = std::stoi(port);
            _LocalIp = "0.0.0.0" ;
            memset(&_Addr , 0 ,sizeof(_Addr));
            _Addr.sin_family = AF_INET ;
            _Addr.sin_port = htons(_Port);
            _Addr.sin_addr.s_addr = INADDR_ANY ;
        }
        bool operator==(const InetAddr& peer) const 
        {
            return peer._LocalIp == _LocalIp &&
            peer._Port == _Port ;
        }
        uint16_t GetInetPort()//返回网络字节序的端口
        {
            return _Addr.sin_port ;
        }
        int GetInetIp()//返回网络字节序的IP
        {
            return _Addr.sin_addr.s_addr ;
        }
        uint16_t GetLocalPort()//返回主机字节序的端口
        {
            return _Port ; 
        }
        std::string GetLocalIp()//返回主机字节序的IP
        {
            return _LocalIp ;
        }
        sockaddr* GetSockaddrPtr()//返回结构体指针
        {
            return (sockaddr*)&_Addr ;
        }
        socklen_t GetSockLine()//返回结构体大小
        {
            socklen_t len = sizeof(_Addr);
            return len ;
        }
    private:
        uint16_t _Port ;//本机端口
        std::string _LocalIp;//本机IP
        sockaddr_in _Addr ;
    };
}
#endif