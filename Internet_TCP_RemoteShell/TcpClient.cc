#include "./BasicElement/Common.hpp"
#include "./BasicElement/InetAddr.hpp"
using namespace InetAddrModule;
int main(int argc, char *argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        LOG(LEVEL::FATAL) << "客户端套接字创建失败" << CAGE;
        exit(SOCKET_ERR);
    }
    InetAddrModule::InetAddr peer(argv[1], argv[2]);
    int m = connect(sockfd, peer.GetSockaddrPtr(),
                    peer.GetSockLine());
    if (m == -1)
    {
        LOG(LEVEL::ERROR) << "客户端向服务器发送连接失败" << CAGE;
        exit(CONNECT__ERR);
    }
    while (true)
    {
        std::string buffer;
        getline(std::cin, buffer);
        int m = write(sockfd, buffer.c_str(), buffer.size());
        if (m > 0)
        {
            LOG(LEVEL::INFO) << "写入内容成功" << CAGE;
            char buf[1024];
            int n = read(sockfd, buf, sizeof(buf));
            if (n > 0)
            {
                buf[n] = 0;
                LOG(LEVEL::INFO) << "读取内容成功" << CAGE;
                std::cout << "The Answer is[" << buf << "]" << CAGE;
            }
            else if (n < 0)
            {
                LOG(LEVEL::ERROR) << "读取内容失败" << CAGE;
                continue;
            }
        }
        else if (m < 0)
        {
            LOG(LEVEL::ERROR) << "写入内容失败" << CAGE;
            continue;
        }
    }
    return 0;
}