#include "./BasicElement/Log.hpp"
#include "./BasicElement/Common.hpp"
#include "./BasicElement/ThreadPool.hpp"
#include "./BasicElement/InetAddr.hpp"
#include "./BasicElement/Route.hpp"
#include "UdpServer.hpp"
using namespace LogModule ;
using namespace ThreadPoolModuleBasedOnSingletonPattern ;
using namespace ChatRoomServerModule ;
using namespace InetAddrModule ;
using namespace RouteModule ;
int main(int argc , char* argv[])
{
    if(argc != 2 )
    {
        LOG(LEVEL::FATAL)<<"输入错误，因该输入./Server 端口号"
        <<CAGE ;
        return 1 ;
    }
    // ./Server 8080
    ENABLE_CONSOLE_LOG_STRATEGY() ;
    uint16_t local_port = std::stoi(argv[1]);

    Route r ;
    ThreadPool<task_t>* tp = ThreadPool<task_t>::GetInstance();
    // void BroadcastMessage(uint16_t sockfd , 
    // std::string message,InetAddr& peer)
    std::unique_ptr<ChatRoomServer> ptr = std::make_unique<ChatRoomServer>
    (local_port , [&r , &tp](uint16_t sockfd , std::string message,InetAddr& peer){
        task_t t = std::bind(&Route::BroadcastMessage ,&r , sockfd , message , peer);
        tp->Equeue(t);
    });
    ptr->Init();
    ptr->Start();
    return 0 ;
}