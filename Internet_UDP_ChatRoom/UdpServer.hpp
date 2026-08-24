#ifndef __DICTSERVER__
#define __DICTSERVER__
#include "./BasicElement/Log.hpp"
#include "./BasicElement/Common.hpp"
using namespace LogModule ;
using namespace InetAddrModule ;
using func_t = std::function<void(uint16_t, const std::string&,
    InetAddrModule::InetAddr&)> ;
namespace ChatRoomServerModule
{
    const int DEFAULT_FD = -1;
    class ChatRoomServer
    {
    public:
        ChatRoomServer(uint16_t port, func_t func)
            : _port(port),
              _func(func),
              _isrunning(false),
              _sockfd(DEFAULT_FD)
        {
        }
        void Init()
        {
            LOG(LEVEL::DEBUG)<<"成功进入服务器初始化"<<CAGE ;
            _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (_sockfd < 0)
            {
                LOG(LEVEL ::ERROR) << "socketfd create fail !" << CAGE;
                exit(1);
            }
            LOG(LEVEL::INFO) << "socketfd create success !" << CAGE;
            sockaddr_in local;
            memset(&local, 0, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(_port);
            local.sin_addr.s_addr = INADDR_ANY;
            socklen_t len = sizeof(local);
            int m = bind(_sockfd, (sockaddr *)&local, len);
            if (m == 0)
                LOG(LEVEL ::INFO) << "bind success !" << CAGE;
            else
            {
                LOG(LEVEL ::ERROR) << "bind error !" << CAGE;
                exit(2);
            }
        }
        void Start()
        {
            LOG(LEVEL::DEBUG)<<"成功进入启动服务器"<<CAGE ;
            _isrunning = true;
            while (_isrunning)
            {
                sockaddr_in peer;
                char buffer_out[1024];
                socklen_t len = sizeof(peer);
                // 接收消息
                int m = recvfrom(_sockfd, buffer_out,
                                 sizeof(buffer_out) - 1, 0, (sockaddr *)&peer, &len);
                if(m > 0)
                {
                    buffer_out[m] = 0;
                    LOG(LEVEL::DEBUG)<<"接收到消息"<<buffer_out<<CAGE;
                    LOG(LEVEL::INFO) << "recvfrom success !" << CAGE;
                    buffer_out[m] = 0;
                    InetAddr client(peer);
                    _func(_sockfd , buffer_out , client);
                }
                else LOG(LEVEL::ERROR) << "recvfrom fail !" << CAGE;
            }
        }
        ~ChatRoomServer()
        {
        }

    private:
        int _sockfd;
        uint16_t _port;
        bool _isrunning;
        func_t _func;
    };
}
#endif