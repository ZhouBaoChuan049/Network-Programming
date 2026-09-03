#include "./BasicElement/Log.hpp"
#include "./BasicElement/Common.hpp"
#include "./BasicElement/InetAddr.hpp"
#include "./BasicElement/Thread.hpp"
#include "./BasicElement/ThreadPool.hpp"
using namespace LogModule;
using namespace InetAddrModule;
using namespace ThreadModule;
using namespace ThreadPoolModuleBasedOnSingletonPattern;
using func_t = std::function<std::string(std::string , InetAddr& user)>;
namespace TcpServerServerModule
{
    const int DEFAULT_BACKLOG = 128;
    class TcpServer : public NoCopy
    {
    public:
        TcpServer(char *port, func_t func)
            : _Port(port),
              _Func(func),
              _isrunning(false)
        {
        }
        void Init()
        {
            _ListenSockPort = socket(AF_INET, SOCK_STREAM, 0);
            if (_ListenSockPort == -1)
            {
                LOG(LEVEL::FATAL) << "监听套接字创建失败" << CAGE;
                exit(SOCKET_ERR);
            }
            LOG(LEVEL::INFO) << "监听套接字创建成功" << CAGE;
            InetAddrModule::InetAddr peer(_Port);
            int n = bind(_ListenSockPort, peer.GetSockaddrPtr(),
                         peer.GetSockLine());
            if (n == -1)
            {
                LOG(LEVEL::FATAL) << "监听套接字绑定失败" << CAGE;
                exit(BIND_ERR);
            }
            LOG(LEVEL::INFO) << "监听套接字绑定成功" << CAGE;
            int sockfd = listen(_ListenSockPort, DEFAULT_BACKLOG);
            if (sockfd < 0)
            {
                LOG(LEVEL::FATAL) << "监听套接字监听失败" << CAGE;
                exit(LISTEN_ERR);
            }
            LOG(LEVEL::INFO) << "监听套接字监听成功" << CAGE;
        }
        void Service()
        {
            _isrunning = true;
            while (_isrunning)
            {
                char buffer[1024];
                int m = read(_sockfd, buffer, sizeof(buffer) - 1);
                if (m > 0)
                {
                    LOG(LEVEL::INFO) << "读取成功，读取到" << buffer << CAGE;
                    buffer[m] = 0;
                    InetAddr peer(_peer);
                    std::string result = _Func(buffer,peer);
                    int n = write(_sockfd, result.c_str(), result.size());
                    if (n < 0)
                    {
                        LOG(LEVEL::ERROR) << "写入数据失败" << CAGE;
                        continue;
                    }
                    else if (n == 0)
                        LOG(LEVEL::INFO) << "未接写入任何数据" << CAGE;
                    else
                        LOG(LEVEL::INFO) << "写入成功" << CAGE;
                }
                else if (m == 0)
                {
                    LOG(LEVEL::INFO) << "对端已经关闭连接" << CAGE;
                    _isrunning = false;
                }
                else
                {
                    LOG(LEVEL::ERROR) << "读取数据失败" << CAGE;
                    continue;
                }
            }
        }
        void Run()
        {
            _isrunning =true ;
            while (_isrunning)
            {
                socklen_t len = sizeof(_peer);
                _sockfd = accept(_ListenSockPort,
                                 (sockaddr*)&_peer, &len);
                if (_sockfd < 0)
                {
                    LOG(LEVEL::ERROR) << "监听套接字获取连接失败" << CAGE;
                    exit(ACCEPT_ERR);
                }
                LOG(LEVEL::ERROR) << "监听套接字获取连接成功" << CAGE;
                // vision-1:单进程版本
                //  Thread thread([this](){
                //      Service();
                //  }) ;

                // vision-2:异步版本
                int pid = fork();
                if (pid == 0)
                {
                    close(_ListenSockPort);
                    int id = fork();
                    if (id > 0)
                    {
                        LOG(LEVEL::INFO) << "子进程退出，孙进程创建" << CAGE;
                        exit(CHILD_ERR);
                    }
                    else if (id == 0)
                    {
                        LOG(LEVEL::INFO) << "孙进程执行任务Service" << CAGE;
                        Service();
                    }
                }
                else if (pid > 0)
                {
                    close(_sockfd);
                    int m = waitpid(pid, nullptr, 0);
                    if (m == -1)
                    {
                        LOG(LEVEL::ERROR) << "父进程等待失败" << CAGE;
                        exit(WAIT_ERR);
                    }
                    LOG(LEVEL::INFO) << "子进程退出成功" << CAGE;
                }
                else
                    LOG(LEVEL::ERROR) << "创建子进程失败" << CAGE;
                // vision3:多线程版本
                //  ThreadPool<task_t>::GetInstance()->Equeue([this](){
                //      Service() ;
                //  });
                //  ThreadPool<task_t>::GetInstance()->Stop();
                //  ThreadPool<task_t>::GetInstance()->Join();
            }
        }

    private:
        char *_Port;
        int _ListenSockPort;
        int _sockfd;
        func_t _Func;
        bool _isrunning;
        sockaddr_in _peer;
    };
}