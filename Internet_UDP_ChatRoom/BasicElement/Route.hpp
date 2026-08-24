#ifndef __ROUTE__
#define __ROUTE__
#include "Common.hpp"
#include "./InetAddr.hpp"
#include "Log.hpp"
using namespace LogModule;
using namespace InetAddrModule ;
namespace RouteModule
{
    class Route
    {
    public:
        Route()
        {}
        ~Route()
        {}
        bool IsExist(InetAddr& peer)
        {
            for(auto e : InteUser)
                if(e == peer)
                    return true ;
            return false ;
        }
        void AddUser(InetAddr& peer)
        {
            InteUser.push_back(peer);
            LOG(LEVEL::INFO)<<"用户["<<peer.StringIP()<<"]加入聊天"
            <<CAGE;
        }
        void DeleteUser(InetAddr& peer)
        {
            auto it = std::find(InteUser.begin(),
            InteUser.end(),peer);
            if(it != InteUser.end())
                InteUser.erase(it);
            LOG(LEVEL::INFO)<<"用户["<<peer.StringIP()<<"]退出"
            <<CAGE;
        }
        void BroadcastMessage(uint16_t sockfd , 
            std::string message,InetAddr& peer)
        {
            if(!IsExist(peer))
                AddUser(peer);
            LOG(LEVEL::DEBUG)<<"NEW WE HAVE A CLIENT NAMED:["<<peer.StringIP()<<"]"<<CAGE ;
            for(int i = 0 ; i < InteUser.size() ; i ++)
            {
                sockaddr_in tmpaddr = InteUser[i].Sockaddr() ;
                sendto(sockfd , message.c_str() , sizeof(message),0,
                (const sockaddr*)&tmpaddr,sizeof(tmpaddr));
                LOG(LEVEL::INFO)<<"发送给一位用户"<<CAGE ;
            }
            if(message == "QUIT")
                DeleteUser(peer);
        }
    private:
        std::vector<InetAddr> InteUser ;
    };
}

#endif