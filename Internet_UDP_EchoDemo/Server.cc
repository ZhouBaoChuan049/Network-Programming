#include "Server.hpp"
using namespace UDPEchodemo ;

const std::string InfoProcess(const char* buffer)
{
    std::stringstream ss ;
    ss<<"Hello!!!!!!!!"<<buffer;
    return ss.str();
}

int main(int argc ,char* argv[] )
{
    if(argc != 2)
    {
        std::cerr<<"输入错误,命令行参数应该是2个"<<std::endl;
        return 1 ;
    }
    uint16_t port = std::stoi(argv[1]);
    //Server server(InfoProcess,port);
    std::unique_ptr<Server> usr = std::make_unique<Server>(InfoProcess,port);
    usr->Init();
    usr->Start();
    return 0;
}