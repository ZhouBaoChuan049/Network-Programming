#ifndef __SELECT_SERVER__
#define __SELECT_SERVER__
#include "Common.hpp"
#include "Log.hpp"
#include "Socket.hpp"
#include "InetAddr.hpp"
using namespace InetAddrModule ;
using namespace LogModule ;
using namespace SocketModule ;

namespace SelectServerModule
{
const int DEFAULT_FD = -1 ;
const size_t SIZE_TMPFD = 1024 ;
    class SelectServer
    {
    public:
        SelectServer(char* port)
            :_Isrunning(false),
             Server(port),
             _ListenSocket(std::make_unique<TcpSocket>())
        {
            for(int i = 0 ; i < SIZE_TMPFD ;i++ )
            {
                _TmpFds[i] = DEFAULT_FD ;
            }
            _ListenSocket->TcpServerSocketMethod(Server);
        }
        void Start()
        {
            _Isrunning = true;
            while(_Isrunning)
            {
                fd_set ReadFdSet ;
                FD_ZERO(&ReadFdSet);
                _TmpFds[0] = _ListenSocket->GetSockFd();
                for(int i = 0 ; i < SIZE_TMPFD ; i++)
                {
                    if(_TmpFds[i] == DEFAULT_FD)
                        continue ;
                    FD_SET(_TmpFds[i] , &ReadFdSet);
                }
                int MaxFd = DEFAULT_FD ;
                for(int i = 0 ; i < SIZE_TMPFD ; i++)
                {
                    if(MaxFd < _TmpFds[i])
                        MaxFd = _TmpFds[i] ;
                }
                struct timeval timeout = {2 , 0} ;
                int n = select(MaxFd+1 , &ReadFdSet , nullptr 
                    , nullptr , &timeout);
                switch(n)
                {
                case -1 :
                    {
                        LOG(LEVEL::ERROR)<<"Select Error"<<CAGE ;
                        exit(SELECT_ERR) ;
                    }
                case  0 :
                    {
                        LOG(LEVEL::INFO)<<"File descriptor not ready for reading" << CAGE;
                        break ;
                    }
                default :
                    {
                        LOG(LEVEL::INFO)<<"File descriptor is ready ! please accept it quickly !"<<CAGE ;
                        HanderEvent(ReadFdSet);
                        break ;
                    }
                }
            }
            _Isrunning = false ;
        }
        void HanderEvent( fd_set FdSet )
        {
            for(int i = 0 ; i < SIZE_TMPFD ; i++)
            {
                if(_TmpFds[i] == DEFAULT_FD)
                    continue ;
                if(FD_ISSET(_TmpFds[i],&FdSet))
                {
                    if(_TmpFds[i] == _ListenSocket->GetSockFd())
                    {
                        InetAddr client ;
                        int fd = _ListenSocket->AcceptSocket(client);
                        if(fd > 0)
                            LOG(LEVEL::DEBUG)<<"获取连接成功，服务套接字fd:"<<fd<<CAGE;
                        for(int i = 0 ; i < SIZE_TMPFD ; i++)
                        {
                            if(_TmpFds[i] == DEFAULT_FD)
                            {
                                LOG(LEVEL::DEBUG)<<"已将文件描述符fd: "<<fd
                                    <<"放入Select的辅助数组中！"<<CAGE ;
                                _TmpFds[i] = fd ;
                                break ;
                            }
                        }
                    }
                    else
                    {
                        char buffer[1024] ;
                        int n = recv(_TmpFds[i] , buffer , sizeof(buffer),0);
                        if(n>0)
                        {
                            LOG(LEVEL::INFO)<<"读取数据成功！"<<CAGE ;
                            std::cout<<"Client Say# ["<<buffer<<"]"<<std::endl ;
                        }
                        else if(n == 0)
                        {
                            LOG(LEVEL::INFO)<<"对端已经关闭连接"<<CAGE ;
                            _TmpFds[i] = DEFAULT_FD ;
                        }
                        else
                        {
                            LOG(LEVEL::ERROR)<<"读取数据错误！"<<CAGE ;
                            continue ;
                        }
                    }
                }
            }
            
        }
        void Stop()
        {
            _Isrunning = false ;
        }
        ~SelectServer()
        {}
    private:
        bool _Isrunning ;
        InetAddr Server ;
        int _TmpFds[SIZE_TMPFD] ;
        std::unique_ptr<Socket> _ListenSocket ;
    };
}



#endif