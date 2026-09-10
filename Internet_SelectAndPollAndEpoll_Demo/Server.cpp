#include "SelectServer.hpp"
#include "PollServer.hpp"
#include "EpollServer.hpp"
#include "Log.hpp"
using namespace SelectServerModule ;
using namespace PollServerModule ;
using namespace EpollServerModule ;
using namespace LogModule ;
int main(int argc ,char* argv[])
{
    ENABLE_CONSOLE_LOG_STRATEGY() ;
    if(argc != 2)
    {
        LOG(LEVEL::FATAL)<<"致命的！你输入的命令行参数错误！应该是 : ./Server port"<<CAGE;
        exit(USAGE_ERR);
    }
    // std::unique_ptr<SelectServer> selectserver = std::make_unique<SelectServer> (argv[1]);
    // selectserver->Start();
    // std::unique_ptr<PollServer> pollserver = std::make_unique<PollServer> (argv[1]);
    // pollserver->Start();
    std::unique_ptr<EpollServer> epollserver = std::make_unique<EpollServer>(argv[1]);
    epollserver->Start();
    return 0;
}