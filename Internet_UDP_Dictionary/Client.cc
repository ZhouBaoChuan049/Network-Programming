#include "Log.hpp"
using namespace LogModule;
int main(int argc, char *argv[])
{
    // 创建套接字，我要给服务器发消息
    uint16_t sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        LOG(LEVEL ::FATAL) << "socket fail !!!" << CAGE;
    else
        LOG(LEVEL::INFO) << "socket success !!!" << CAGE;
    sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    //./Client ip port
    uint16_t local_port = std::stoi(argv[2]);
    server.sin_port = htons(local_port);
    inet_pton(AF_INET, argv[1], &server.sin_addr);
    socklen_t _len = sizeof(server);
    while (true)
    {
        std::string _buffer;
        std::getline(std::cin, _buffer);
        int m = sendto(sockfd, _buffer.c_str(), _buffer.size(), 0,
                       (sockaddr *)&server, _len);
        if (m > 0)
        {
            LOG(LEVEL::INFO) << "sendto success!!" << CAGE;
            char buffer[1024];
            sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int n = recvfrom(sockfd, buffer, sizeof(buffer),
                             0, (sockaddr *)&peer, &len);
            buffer[n] = 0;
            if (n > 0)
                std::cout << "The Translate answer is :[" << buffer << "]"
                          << std::endl;
            else
                LOG(LEVEL::ERROR) << "recvfrom fail !!" << CAGE;
        }
        else if (m == -1)
            LOG(LEVEL ::ERROR) << "sendto fail !!" << CAGE;
    }
    return 0;
}