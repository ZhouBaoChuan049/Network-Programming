#include "./BasicElement/Common.hpp"
#include "./BasicElement/Log.hpp"
#include "./BasicElement/InetAddr.hpp"
#include "TcpServer.hpp"
using namespace LogModule;
using namespace TcpServerServerModule;
using namespace InetAddrModule;
std::string DefaultFunc(std::string str)
{
    std::stringstream ss;
    ss << "Client Say:" << "[" << str << "]" << std::endl;
    return ss.str();
}

void Usage()
{
    LOG(LEVEL::FATAL) << "命令行参数输入错误" << "应该是:[" << "./Server" << "port" << "]" << CAGE;
}
int main(int argc, char *argv[])
{
    //./Server 8080
    if (argc != 2)
    {
        Usage();
        exit(USAGE_ERR);
    }
    Dictionary dict;
    dict.LoadDict();
    std::unique_ptr<TcpServer> ptr =
        std::make_unique<TcpServer>(argv[1], [&dict](std::string english)->std::string {
            return dict.Translate(english);
        });
    ptr->Init();
    ptr->Run();
    return 0;
}
