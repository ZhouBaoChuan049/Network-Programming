#include "Log.hpp"
#include "Common.hpp"
#include "InetAddr.hpp"
#include "Socket.hpp"
#include "TcpServer.hpp"
#include "NetCal.hpp"
#include "Protocol.hpp"
#include "Daemon.hpp"
using namespace LogModule ;
using namespace InetAddrModule ;
using namespace SocketModule ;
using namespace TcpServerModule ;
using namespace NetCalModule ;
using namespace ProtocolModule ;
using namespace DaemonModule ;
void Usage()
{
    LOG(LEVEL::FATAL)<<"命令行参数输入错误"<<"正确输入格式: "<<
    "[./Server xxxx(PORT)]"<<CAGE ;
}
int main(int argc ,char* argv[])
{
    if(argc != 2)
    {
        Usage();
        exit(USAGE_ERR);
    }
    EnableDaemon(0,0);
    ENABLE_FILE_LOG_STRATEGY();
    std::shared_ptr<NetCal> cal = std::make_shared<NetCal>();
    std::shared_ptr<Protocol> protocol= 
            std::make_shared<Protocol>([&cal](Request& req)->Respone{
                return cal->Execute(req);
    });
    std::shared_ptr<TcpServer> server = 
            std::make_shared<TcpServer>(argv[1] , [&protocol](std::shared_ptr<Socket>& sock , InetAddr& client){
                protocol->GetRequest(sock , client);
    });
    server->Start();
    return 0;
}