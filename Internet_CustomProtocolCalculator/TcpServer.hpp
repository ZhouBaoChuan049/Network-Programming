#ifndef __TCP_SERVER__
#define __TCP_SERVER__
#include "Log.hpp"
#include "Common.hpp"
#include "InetAddr.hpp"
#include "Socket.hpp"

using namespace LogModule;
using namespace InetAddrModule;
using namespace SocketModule;
using service_t = std::function<void(std::shared_ptr<Socket> &, InetAddr &)>;
namespace TcpServerModule
{
    class TcpServer : public NoCopy
    {
    public:
        TcpServer()
        {
        }
        TcpServer(char *port, service_t service)
            : _port(port),
              _isrunning(false),
              _service(service)
        {
        }
        ~TcpServer()
        {
        }
        void Start()
        {
            std::shared_ptr<Socket> sock =
                std::make_shared<TcpSocket>();
            LOG(LEVEL::DEBUG) << "服务器创建套接字成功" << CAGE;
            InetAddr server(_port);
            sock->TcpServerSocketMethod(server);
            LOG(LEVEL::DEBUG) << "服务器初始化套接字成功" << CAGE;
            _isrunning = true;
            while (_isrunning)
            {
                InetAddr client;
                std::shared_ptr<Socket> Sptr = sock->AcceptSocket(client);
                int pid = fork();
                if (pid == 0)
                {
                    LOG(LEVEL::DEBUG) << "服务器创建子进程成功" << CAGE;
                    int m = fork();
                    if (m > 0)
                        exit(OK);
                    LOG(LEVEL::DEBUG) << "进入 service" << CAGE;

                    _service(Sptr, client);

                    LOG(LEVEL::DEBUG) << "service 已经返回" << CAGE;
                }
                else
                {
                    int n = waitpid(pid, nullptr, 0);
                    if (n == -1)
                        LOG(LEVEL::ERROR) << "父进程等待子进程错误" << CAGE;
                }
            }
            _isrunning = false;
        }

    private:
        char *_port;
        service_t _service;
        bool _isrunning;
    };
}

#endif