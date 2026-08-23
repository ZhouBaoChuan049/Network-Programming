#ifndef __DICTSERVER__
#define __DICTSERVER__
#include "Log.hpp"
using namespace LogModule;
using func_t = std::function<std::string(const std::string &)>;
namespace DictionaryServer
{
    const int DEFAULT_FD = -1;
    class DictServer
    {
    public:
        DictServer(uint16_t port, func_t func)
            : _port(port),
              _func(func),
              _isrunning(false),
              _sockfd(DEFAULT_FD)
        {
        }
        void Init()
        {
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
            local.sin_port = _port;
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
            _isrunning = true;
            while (_isrunning)
            {
                sockaddr_in peer;
                char buffer_out[1024];
                socklen_t len = sizeof(peer);
                // 接收消息
                int m = recvfrom(_sockfd, &buffer_out,
                                 sizeof(buffer_out) - 1, 0, (sockaddr *)&peer, &len);
                if (m == -1)
                {
                    LOG(LEVEL::ERROR) << "recvfrom fail !" << CAGE;
                }
                else if (m > 0)
                {
                    LOG(LEVEL::INFO) << "recvfrom success !" << CAGE;
                    buffer_out[m] = 0;
                    // LOG(LEVEL::DEBUG)<<buffer_out<<CAGE;
                    // 注入函数，得到翻译结果
                    std::string result = _func(buffer_out);

                    // 发送翻译结果
                    int n = sendto(_sockfd, result.c_str(), result.size(),
                                   0, (sockaddr *)&peer, len);
                    if (n == -1)
                        LOG(LEVEL::ERROR) << "sendto fail !" << CAGE;
                    else if (n > 0)
                        LOG(LEVEL::INFO) << "senfto success !" << CAGE;
                }
            }
        }
        ~DictServer()
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