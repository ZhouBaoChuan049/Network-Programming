#include "Log.hpp"
using namespace LogModule ;
using func_t = std::function<const std::string(const char*)>;
namespace UDPEchodemo
{
    class Server
    {
    public:
        Server(func_t func , uint16_t port)
            :_func(func),
            _port(port),
            _socketfd(0),
            _isrunning(false)
        {}
        void Init()
        {
            _socketfd = socket(AF_INET , SOCK_DGRAM , 0);
            if(_socketfd < 0)
            {
                LOG(LEVEL::ERROR) << "socket fail !";
                exit(1);
            }
            LOG(LEVEL::DEBUG)<<"socket success !";
            struct sockaddr_in local ;
            memset(&local , 0 , sizeof(local) );
            local.sin_family = AF_INET ;
            local.sin_port = htons(_port);
            local.sin_addr.s_addr = INADDR_ANY ;
            int n = bind(_socketfd ,(struct sockaddr*)&local,sizeof(local));
            if(n < 0)
            {
                LOG(LEVEL::FATAL)<<"bind fail !";
                exit(2);
            }
            LOG(LEVEL::DEBUG)<<"bind success !";
        }
        void Start()
        {
            _isrunning = true ;
            while(_isrunning)
            {
                char buffer[1024] ;
                struct sockaddr_in peer ;
                socklen_t len = sizeof(peer);
                ssize_t n = recvfrom(_socketfd , buffer , sizeof(buffer)-1 ,0,(struct sockaddr*)&peer,&len);
                if(n > 0)  
                {
                    int peer_port = ntohs(peer.sin_port);
                    std::string peer_ip = inet_ntoa(peer.sin_addr);
                    buffer[n] = 0;
                    std::string result = _func(buffer);
                    int n = sendto(_socketfd , result.c_str(),result.size(),0,(struct sockaddr*)&peer,len);
                    if(n > 0) LOG(LEVEL::INFO) <<"sendto success"<<CAGE ;
                    else LOG(LEVEL::ERROR)<<"sendto fail"<<CAGE;
                }
            }
        }
        ~Server(){}
    private:
        int _socketfd ;
        func_t _func ;
        uint16_t _port;
        bool _isrunning ;
    };
}