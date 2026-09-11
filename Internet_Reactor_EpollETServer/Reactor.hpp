#ifndef __REACTOR__
#define __REACTOR__
#include "Common.hpp"
#include "Connection.hpp"
#include "Log.hpp"
#include "Epoller.hpp"
using namespace LogModule ;
using namespace ConnectionModule ;
using namespace EpollerModule ;
namespace ReactorModule
{
const int TIMEOUT = -1;
const int MAX_EVENT = 1024 ;
    class Reactor
    {
    public:
        Reactor()
            :_IsRunning(false),
             _Epol(std::make_unique<Epoller>())
        {}
        bool IsExistInConnections(int sockfd)
        {
            return _Connections.find(sockfd) == 
                _Connections.end() ? false : true;
        }
        bool AddConnection(std::shared_ptr<Connection> conn)
        {
            LOG(LEVEL::DEBUG)<<"到这里了嘛?"<<CAGE;
            if(IsExistInConnections(conn->Getsocketfd()))
            {
                LOG(LEVEL::INFO)<<"该文件描述符链接已经被维护"<<CAGE ;
                return false;                
            }
            else
            {
                //写透入内核
                epoll_event event ;
                event.data.fd = conn->Getsocketfd();
                event.events = conn->GetEvent();
                _Epol->EpollAdd(conn->Getsocketfd() , event);
                //加入组织
                if(!IsExistInConnections(conn->Getsocketfd()))
                    _Connections.insert
                        (std::make_pair(conn->Getsocketfd(),conn));
                conn->SetOwner(this);
            }
            PrintConnections();
            return true;
        }
        void DelConnection(int sockfd)
        {
            _Epol->EpollDel(sockfd);
            close(sockfd);
            _Connections.erase(sockfd);
        }
        int LoopOnce(int timeout)
        {
            int nfd = _Epol->EpollerWait(_Eevents, MAX_EVENT, timeout);
            if(nfd > 0) 
            {
                LOG(LEVEL::INFO)<<
                    "_Epol->EpollerWait等待文件描述符成功! nfd:["<<nfd<<"]"<<CAGE;
                return nfd ;
            }
            else if(nfd == 0) 
            {
                LOG(LEVEL::INFO)<<
                    "_Epol->EpollerWait等待文件描述符超时!"<<CAGE;
                return 0 ;
            }
            else if(nfd <0 && errno == EINTR)
            {
                LOG(LEVEL::ERROR)<<
                    "_Epol->EpollerWait等待文件描述符被信号中断!"<<CAGE;
                return -1;
            }
            else
            {
                LOG(LEVEL::ERROR)<<
                    "_Epol->EpollerWait等待文件描述符出错!"<<CAGE;
                return -1 ;
            }
        }
        void Dispatcher(int n)
        {
            if(n <= 0)
                return ;
            for(int i =0; i < n; i ++)
            {
                //LOG(LEVEL::DEBUG)<<"到这里了没有？"<<CAGE;
                uint32_t event = _Eevents[i].events;
                int sockfd = _Eevents[i].data.fd;
                //LOG(LEVEL::DEBUG)<<"到这里了没有？"<<event <<sockfd <<CAGE;
                if(event&EPOLLERR)
                    { event |= EPOLLIN | EPOLLOUT ; }
                if(event&EPOLLHUP ||event&EPOLLRDHUP )
                    { event |= EPOLLIN | EPOLLOUT ; }
                if(event&EPOLLIN)
                {
                    //LOG(LEVEL::DEBUG)<<"到这里了没有？"<<CAGE;
                    if(IsExistInConnections(sockfd))
                        _Connections[sockfd]->Recv();
                }
                if(event&EPOLLOUT)
                {
                    if(IsExistInConnections(sockfd))
                        _Connections[sockfd]->Send();
                }
            }
        }
        void Start()
        {
            _IsRunning = true ;
            while(_IsRunning)
            {
               int n = LoopOnce(TIMEOUT);
               if(n == 0)
                    continue ;
                else if(n == -1)
                    break ;
               Dispatcher(n);
            }
            _IsRunning = false ;
        }
        void TakeCareOfInOut(int sockfd , uint32_t reader, uint32_t writer)
        {
            _Epol->EpollChMod(sockfd , reader | writer);
        }
        void PrintConnections()
        {
            for(auto conns : _Connections)
                std::cout<<"conns.first: ["<<conns.first<<"]"<<std::endl;
        }
    private:
        bool _IsRunning ;
        std::unordered_map<int , std::shared_ptr<Connection>> _Connections ;
        std::unique_ptr<Epoller> _Epol ;
        epoll_event _Eevents[MAX_EVENT] ;
    };
}
#endif