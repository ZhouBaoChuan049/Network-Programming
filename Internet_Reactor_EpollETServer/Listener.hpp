#ifndef __LISTENER__
#define __LISTENER__
#include "Common.hpp"
#include "Connection.hpp"
#include "Epoller.hpp"
#include "Socket.hpp"
#include "InetAddr.hpp"
#include "Channel.hpp"
#include "Reactor.hpp"
using namespace ConnectionModule;
using namespace EpollerModule;
using namespace SocketModule;
using namespace InetAddrModule;
using namespace ChannelModule;
using namespace ReactorModule;
namespace ListenerModule
{
    const int DEFAULT_FD = -1;
    class Listener : public Connection
    {
    public:
        Listener(char* port)
            : _listensocketfd(std::make_unique<TcpSocket>()),
              _port(port)
        {
            InetAddr Server(_port);
            //获取这个网络套接字
            _listensocketfd->TcpServerSocketMethod(Server);
            //设置关系事件是什么
            SetEvent(EPOLLIN|EPOLLET);
            //把文件描述符变成非阻塞
            int flag = fcntl(_listensocketfd->GetSockFd() , F_GETFL,0);
            fcntl(_listensocketfd->GetSockFd(),F_SETFL ,flag|O_NONBLOCK);
        }
        int Getsocketfd() override
        {
            return _listensocketfd->GetSockFd();
        }
        void Recv() override
        {
            LOG(LEVEL::DEBUG)<<"到这里了嘛?"<<CAGE;
            while(true)
            {
                InetAddr client ;
                int sockfd = _listensocketfd->AcceptSocket(client);
                //LOG(LEVEL::DEBUG)<<"到这里了没有？"<<sockfd<<CAGE;
                if(errno & EAGAIN || errno | EWOULDBLOCK)
                {
                    LOG(LEVEL::DEBUG)<<"到这里了嘛?"<<CAGE;
                    break ;
                }
                if(errno & EINTR)
                {
                    LOG(LEVEL::DEBUG)<<"到这里了嘛?"<<CAGE;
                    continue ;
                }
                if(sockfd>0)
                {
                    LOG(LEVEL::DEBUG)<<"到这里了嘛?"<<CAGE;
                    std::shared_ptr<Connection> channel = 
                        std::make_shared<Channel>(sockfd);
                    channel->SetEvent(EPOLLIN|EPOLLET);
                    int flag = fcntl(channel->Getsocketfd() , F_GETFL,0);
                    fcntl(channel->Getsocketfd(),F_SETFL ,flag|O_NONBLOCK);
                    GetOwner()->AddConnection(channel);
                }
                else 
                {
                    LOG(LEVEL::ERROR)<<"错误!获取监听套接字失败。"<<CAGE;
                    exit(ACCEPT_ERR);
                }
            }
        }
        void Send()override{} 
    private:
        const char* _port ;
        
        std::unique_ptr<TcpSocket> _listensocketfd;
        
    };
}

#endif