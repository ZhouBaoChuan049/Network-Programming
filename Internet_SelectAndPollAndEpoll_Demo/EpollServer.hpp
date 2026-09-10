#ifndef __EPOLL_SERVER__
#define __EPOLL_SERVER__
#include "Common.hpp"
#include "Log.hpp"
#include "Socket.hpp"
#include "InetAddr.hpp"
using namespace InetAddrModule;
using namespace LogModule;
using namespace SocketModule;

namespace EpollServerModule
{
const int EPOLL_SIZE = 128 ;
const int READY_EVENT = 1024 ;
const int BUFFER_SIZE = 1024 ;
    class EpollServer
    {
    public:
        EpollServer(char* port)
            :_Isrunning(false),
             _ListenSocket(std::make_unique<TcpSocket>()),
             _Server(port)
        {
            _ListenSocket->TcpServerSocketMethod(_Server);
            _efd = epoll_create(EPOLL_SIZE);
            epoll_event event ;
            event.data.fd = _ListenSocket->GetSockFd();
            event.events = EPOLLIN ;
            epoll_ctl(_efd , EPOLL_CTL_ADD , _ListenSocket->GetSockFd() , &event);
        }
        void Start()
        {
            _Isrunning = true;
            while(_Isrunning)
            {
                int nfd = epoll_wait(_efd , _ReadyFd , READY_EVENT , 2000 );
                if(nfd < 0)
                    LOG(LEVEL::ERROR)<<"等待文件描述符出现错误！"<<CAGE ;
                else if(nfd == 0)
                    LOG(LEVEL::INFO)<<"没有就绪的文件描述符，等待超时"<<CAGE ;
                else 
                {
                    LOG(LEVEL::INFO)<<"有文件描述符就绪，请立即处理！就绪数: "<<nfd <<CAGE ;
                    Dispatcher(nfd) ;
                }
            }
            _Isrunning = false ;
        }
        void Dispatcher(int nfd)
        {
            for(int i = 0 ; i < nfd ; i ++)
            {
                if(_ReadyFd[i].data.fd == _ListenSocket->GetSockFd())
                    Accepter() ;
                else 
                    Recver(_ReadyFd[i].data.fd) ;
            }
        }
        void Recver(int fd)
        {
            char buffer [BUFFER_SIZE] ;
            int n = recv(fd , buffer , sizeof(buffer) , 0);
            if(n > 0)
            {
                LOG(LEVEL::INFO)<<"读取内容成功"<<CAGE ;
                std::cout<<"Client Say# ["<<buffer<<"]"<<CAGE ;
            }
            else if(n == 0)
            {
                LOG(LEVEL::INFO)<<"对端已关闭连接"<<CAGE ;
                epoll_ctl(_efd , EPOLL_CTL_DEL , fd , nullptr);
            }
            else 
                LOG(LEVEL::ERROR)<<"读取内容失败"<<CAGE ;
        }
        void Accepter()
        {
            InetAddr client ;
            int newfd = _ListenSocket->AcceptSocket(client);
            if(newfd > 0)
            {
                epoll_event event ;
                event.data.fd = newfd ;
                event.events = EPOLLIN ;
                epoll_ctl(_efd , EPOLL_CTL_ADD , newfd , &event);
            }
        }
        void Stop()
        {
            _Isrunning = false ;
        }
        ~EpollServer()
        {}
    private:
        bool _Isrunning ;
        int _efd ;
        epoll_event _ReadyFd [READY_EVENT] ;
        InetAddr _Server ;
        std::unique_ptr<Socket> _ListenSocket ;
    };
}
#endif