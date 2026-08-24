#include "./BasicElement/Log.hpp"
#include "./BasicElement/Common.hpp"
#include "./BasicElement/Thread.hpp"
using namespace LogModule ;
using namespace ThreadModule ;
uint16_t sockfd = 0 ;
uint16_t local_port = 0 ;
sockaddr_in server ;
void ReadInternet()
{
    socklen_t _len = sizeof(server);
    while (true)
    {
        std::string _buffer;
        std::cout<<"#PleaseEnter: " ;
        std::getline(std::cin, _buffer);
        int m = sendto(sockfd, _buffer.c_str(), _buffer.size(), 0,
                       (sockaddr *)&server, _len);
        if(m>0)LOG(LEVEL::INFO)<<"Sendto Success! "<<CAGE ;
        else LOG(LEVEL::ERROR)<<"Sendto Fail! "<<CAGE;
    }
}

void WriteInternet()
{
    while(true)
    {
        char buffer[1024];
        sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int n = recvfrom(sockfd, buffer, sizeof(buffer),
                            0, (sockaddr *)&peer, &len);
        if(n>0)LOG(LEVEL::INFO)<<"Recvfrom Success! "<<CAGE ;
        else LOG(LEVEL::ERROR)<<"Recvfrom Fail! "<<CAGE;
    }
}

int main(int argc, char *argv[])
{
    // 创建套接字，我要给服务器发消息
    ENABLE_CONSOLE_LOG_STRATEGY() ;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        LOG(LEVEL ::FATAL) << "socket fail !!!" << CAGE;
    else
        LOG(LEVEL::INFO) << "socket success !!!" << CAGE;
    
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    local_port = std::stoi(argv[2]);
    server.sin_port = htons(local_port);
    inet_pton(AF_INET, argv[1], &server.sin_addr);
    Thread Reader(ReadInternet);
    Thread Writer(WriteInternet);

    Reader.Start();
    Writer.Start();

    Reader.join();
    Writer.join();
    return 0;
}