#include "./BasicElement/Log.hpp"
#include "./BasicElement/Common.hpp"
#include "./BasicElement/InetAddr.hpp"
#include "./BasicElement/Socket.hpp"
#include "TcpServer.hpp"
#include "Http.hpp"
using namespace LogModule ;
using namespace InetAddrModule ;
using namespace SocketModule ;
using namespace TcpServerModule ;
using namespace HttpModule ;
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
    ENABLE_FILE_LOG_STRATEGY();
    Http http ;
    http.Start(argv[1]);
    return 0;
}