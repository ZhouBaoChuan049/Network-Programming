#ifndef __EPOLLER__
#define __EPOLLER__
#include "Common.hpp"
#include "Log.hpp"
using namespace LogModule ;
namespace EpollerModule
{
const int EPOLL_CREATESIZE = 128 ;
    class Epoller
    {
    public:
        Epoller()
        {
            _efd = epoll_create(EPOLL_CREATESIZE);
            if(_efd<0)  LOG(LEVEL::FATAL)<<"致命!创建EPOLL模型错误。"<<CAGE;
            else LOG(LEVEL::INFO)<<"创建EPOLL模型成功!"<<CAGE ;
        }
        void EpollAdd(int sockfd , epoll_event event)
        {
            int n = epoll_ctl(_efd , EPOLL_CTL_ADD ,
                    sockfd , &event);
            if(n == 0) LOG(LEVEL::INFO)<<"Epoll添加结点成功。"<<CAGE ;
            else LOG(LEVEL::ERROR)<<"错误!Epoll添加结点失败。"<<CAGE ;
        }
        void EpollDel(int sockfd)
        {
            int n = epoll_ctl(_efd , EPOLL_CTL_DEL,
                    sockfd , nullptr);
            if(n == 0) LOG(LEVEL::INFO)<<"Epoll删除结点成功。"<<CAGE ;
            else LOG(LEVEL::ERROR)<<"错误!Epoll删除结点失败。"<<CAGE ;
        }
        int EpollerWait(epoll_event* event , 
                int maxevents , int timeout){
            return epoll_wait(_efd ,event, maxevents , timeout );
        }
        void EpollChMod(int sockfd, uint16_t mode)
        {
            epoll_event event;
            event.data.fd = sockfd ;
            event.events = mode ;
            int n = epoll_ctl(_efd ,EPOLL_CTL_MOD, sockfd, &event);
            if(n == 0) LOG(LEVEL::INFO)<<"Epoll修改关心事件成功。"<<CAGE ;
            else LOG(LEVEL::ERROR)<<"错误!Epoll修改关心事件失败。"<<CAGE ;            
        }
        ~Epoller()
        {
        }
    private:
        int _efd ;
    };
}


#endif
