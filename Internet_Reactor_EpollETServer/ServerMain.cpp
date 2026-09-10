#include "Common.hpp"
#include "Log.hpp"
#include "NetCal.hpp"
#include "Protocol.hpp"
#include "Connection.hpp"
#include "Listener.hpp"
#include "Reactor.hpp"
using namespace ProtocolModule ;
using namespace NetCalModule ;
using namespace LogModule ;
using namespace ConnectionModule ;
using namespace ListenerModule ;
using namespace ReactorModule ;
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        LOG(LEVEL::FATAL) << "命令行参数输入错误!应当为[./Server port]" << CAGE;
        return 1;
    }
    ConsoleLogStrategy();
    char* port = argv[1];
    std::unique_ptr<NetCal> netcal =
        std::make_unique<NetCal>();
    std::unique_ptr<Protocol> protocol = std::make_unique<Protocol>(
        [&netcal](Request &req) -> Respone{
            return netcal->Execute(req);
    });
    std::shared_ptr<Connection> conn = std::make_shared<Listener>(port);
    conn->SetCallBackFunc(
        [&protocol](std::string inbuffer)->std::string{
            std::string response;
            while(true)
            {
                std::string package ;
                if(!protocol->decode(inbuffer, &package))
                    break;
                response += protocol->Execute(package);
            }
            return response;
        }
    );
    std::shared_ptr<Reactor> reactor = std::make_shared<Reactor>();
    reactor->AddConnection(conn);
    reactor->Start();
    return 0;
}