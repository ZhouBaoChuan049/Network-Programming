#ifndef __INETADDR__
#define __INETADDR__
#include "Log.hpp"
#include "Common.hpp"
namespace InetAddrModule
{
    class InetAddr
    {
    public:
        InetAddr(sockaddr_in& saddr)
            :_saddr(saddr)
        {
            //网络序列转本地序列
            _port = ntohs(saddr.sin_port);
            _LocalIp = inet_ntoa(saddr.sin_addr);
        }
        uint16_t Port ()const{ return _port ;}
        std::string StringIP ()const{ return _LocalIp ;}
        sockaddr_in Sockaddr(){ return _saddr ;}
        bool operator==(const InetAddr& peer) const
        {
            return peer.Port() == _port &&
             peer.StringIP()==_LocalIp;
        }
        ~InetAddr(){}
    private:
        uint16_t _port ;
        std::string _LocalIp ;
        sockaddr_in _saddr ;
    };
}
#endif