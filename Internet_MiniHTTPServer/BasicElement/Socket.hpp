#ifndef __SOCKET__
#define __SOCKET__
#include "Log.hpp"
#include "Common.hpp"
#include "InetAddr.hpp"
using namespace LogModule;
using namespace InetAddrModule;

namespace SocketModule
{
    const int DEFAULT_BACKLOG = 258;
    class Socket
    {
    public:
        virtual void CreateSocket() = 0;
        virtual bool BindSocket(InetAddr &Server) = 0;
        virtual bool ListenSocket(int backlog = DEFAULT_BACKLOG) = 0;
        virtual bool ConnectSocket(InetAddr &peer) = 0;
        virtual std::shared_ptr<Socket> AcceptSocket(InetAddr &client) = 0;
        virtual int RecvMessage(std::string *Message) = 0;
        virtual bool SendMessage(std::string *Message) = 0;
        virtual int GetSockFd() = 0;
        virtual void Close() = 0;

    public:
        void TcpClientSocketMethod()
        {
            CreateSocket();
        }
        void TcpServerSocketMethod(InetAddr &Server,
                                   int backlog = DEFAULT_BACKLOG)
        {
            CreateSocket();
            BindSocket(Server);
            ListenSocket(backlog);
        }

    private:
        int _listensockfd;
    };
    class TcpSocket : public Socket
    {
    public:
        TcpSocket(int listensockfd = 0)
            : _listensockfd(listensockfd)
        {
        }
        ~TcpSocket()
        {
        }
        void CreateSocket()
        {
            _listensockfd = ::socket(AF_INET, SOCK_STREAM, 0);
            if (_listensockfd < 0)
            {
                LOG(LEVEL::FATAL) << "致命！套接字创建失败。" << CAGE;
                exit(SOCKET_ERR);
            }
        }
        bool BindSocket(InetAddr &Server)
        {
            int n = ::bind(_listensockfd, Server.GetSockaddrPtr(),
                           Server.GetSockLine());
            if (n == -1)
            {
                LOG(LEVEL::FATAL) << "致命！绑定套接字失败。" << CAGE;
                exit(BIND_ERR);
            }
            return true;
        }
        bool ListenSocket(int backlog = DEFAULT_BACKLOG)
        {
            int n = ::listen(_listensockfd, backlog);
            if (n == -1)
            {
                LOG(LEVEL::FATAL) << "致命！监听套接字失败。" << CAGE;
                exit(LISTEN_ERR);
            }
            return true;
        }
        bool ConnectSocket(InetAddr &peer)
        {
            int n = ::connect(_listensockfd, peer.GetSockaddrPtr(),
                              peer.GetSockLine());
            if (n == -1)
            {
                LOG(LEVEL::FATAL) << "客户端连接服务器失败！" << CAGE;
                exit(CONNECT__ERR);
            }
            return true;
        }
        std::shared_ptr<Socket> AcceptSocket(InetAddr &client)
        {
            sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int _sockfd = accept(_listensockfd, (sockaddr *)&peer, &len);
            if (_sockfd < 0)
            {
                LOG(LEVEL::FATAL) << "服务器获取连接失败！" << CAGE;
                exit(ACCEPT_ERR);
            }
            std::shared_ptr<Socket> ptr =
                std::make_shared<TcpSocket>(_sockfd);
            client.SetAddr(peer);
            return ptr;
        }
        int RecvMessage(std::string *Message)
        {
            int n = recv(GetSockFd(), Message->data(), Message->size(), 0);
            if (n > 0)
                Message->resize(n);
            else
                Message->clear();
            return n;
        }
        bool SendMessage(std::string *Message)
        {
            LOG(LEVEL::DEBUG) << "如果能看到这句，说明进入了发送请求串" << CAGE;
            size_t total = 0;
            size_t size = Message->size();
            while (total < size)
            {
                ssize_t n = send(GetSockFd(),Message->data()
                         + total,size - total,0);
                if (n < 0){
                    LOG(LEVEL::DEBUG) << "发送请求串失败" << CAGE;
                    return false;
                }
                total += n;
            }
            LOG(LEVEL::DEBUG) << "发送请求串成功" << CAGE;
            return true;
        }
        int GetSockFd() { return _listensockfd; }
        void Close() { close(_listensockfd); }

    private:
        int _listensockfd;
    };
}

#endif