#ifndef __POLL_SERVER__
#define __POLL_SERVER__
#include "Common.hpp"
#include "Log.hpp"
#include "Socket.hpp"
#include "InetAddr.hpp"
using namespace InetAddrModule;
using namespace LogModule;
using namespace SocketModule;
namespace PollServerModule
{
const int DEFAULT_SIZE = 1024;
const int DEFAULT_FD = -1 ;
    class PollServer
    {
    public:
        PollServer(char* port)
            : _Isrunning(false),
              Server(port),
              _ListenSocket(std::make_unique<TcpSocket>())
        {
            _ListenSocket->TcpServerSocketMethod(Server);
            for(int i = 0 ; i < DEFAULT_SIZE ; i++)
            {
                _PollFds[i].fd = DEFAULT_FD ;
            }
            _PollFds[0].fd =_ListenSocket->GetSockFd() ;
            _PollFds[0].events = POLL_IN ;
        }
        void Start()
        {
            _Isrunning = true;
            while (_Isrunning)
            {
                int n = poll(_PollFds, DEFAULT_SIZE, 2000);
                switch (n)
                {
                    case -1:
                    {
                        LOG(LEVEL::ERROR) << "Poll Error" << CAGE;
                        exit(SELECT_ERR);
                    }
                    case 0:
                    {
                        LOG(LEVEL::INFO) << "File descriptor not ready for reading" << CAGE;
                        break;
                    }
                    default:
                    {
                        LOG(LEVEL::INFO) << "File descriptor is ready ! please accept it quickly !" << CAGE;
                        HanderEvent();
                        break;
                    }
                }
            }
            _Isrunning = false;
        }
        void HanderEvent()
        {
            for (int i = 0; i < DEFAULT_SIZE; i++)
            {
                if (_PollFds[i].fd == DEFAULT_FD)
                    continue;
                if (_PollFds[i].revents & POLL_IN)
                {
                    if (_PollFds[i].fd == _ListenSocket->GetSockFd())
                    {
                        InetAddr client;
                        int fd = _ListenSocket->AcceptSocket(client);
                        if (fd > 0)
                            LOG(LEVEL::DEBUG) << "获取连接成功，服务套接字fd:" << fd << CAGE;
                        for (int i = 0; i < DEFAULT_SIZE; i++)
                        {
                            if (_PollFds[i].fd == DEFAULT_FD)
                            {
                                LOG(LEVEL::DEBUG) << "已将文件描述符fd: " << fd
                                                  << "放入Select的辅助数组中！" << CAGE;
                                _PollFds[i].fd = fd;
                                _PollFds[i].events = POLL_IN ;
                                break;
                            }
                        }
                    }
                    else
                    {
                        char buffer[1024];
                        int n = recv(_PollFds[i].fd, buffer, sizeof(buffer), 0);
                        if (n > 0)
                        {
                            buffer[n] = 0;
                            LOG(LEVEL::INFO) << "读取数据成功！" << CAGE;
                            std::cout << "Client Say# [" << buffer << "]" << std::endl;
                        }
                        else if (n == 0)
                        {
                            LOG(LEVEL::INFO) << "对端已经关闭连接" << CAGE;
                            _PollFds[i].fd = DEFAULT_FD;
                        }
                        else
                        {
                            LOG(LEVEL::ERROR) << "读取数据错误！" << CAGE;
                            continue;
                        }
                    }
                }
            }
        }
        void Stop()
        {
            _Isrunning = false;
        }
        ~PollServer()
        {
        }

    private:
        bool _Isrunning;
        InetAddr Server;
        struct pollfd _PollFds[DEFAULT_SIZE];
        std::unique_ptr<Socket> _ListenSocket;
    };
}
#endif