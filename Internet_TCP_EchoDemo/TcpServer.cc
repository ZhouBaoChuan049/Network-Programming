#include "./BasicElement/Common.hpp"
#include "./BasicElement/Log.hpp"
#include "./BasicElement/InetAddr.hpp"
#include "TcpServer.hpp"
using namespace LogModule ;
using namespace TcpServerServerModule ;
using namespace InetAddrModule ;
std::string DefaultFunc(std::string str)
{
    std::stringstream ss ;
    ss<<"Client Say:"<<"["<<str<<"]"<<std::endl;
    return ss.str();
}

void Usage(char* port)
{
    LOG(LEVEL::FATAL)<<"命令行参数输入错误"<<
    "应该是:["<<"./Server"<<port<<"]"<<CAGE ;
}
int main(int argc , char* argv[])
{
    //./Server 8080
    if(argc != 2){
        Usage(argv[1]);
        exit(USAGE_ERR);
    }
    std::unique_ptr<TcpServer> ptr = 
        std::make_unique<TcpServer>(argv[1] , DefaultFunc);
    ptr->Init();
    ptr->Run();
    return 0 ;
}
