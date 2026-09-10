#include "Log.hpp"
#include "Common.hpp"
#include "InetAddr.hpp"
#include "Socket.hpp"
#include "Protocol.hpp"
using namespace LogModule ;
using namespace InetAddrModule ;
using namespace SocketModule ;
using namespace ProtocolModule ;
void Usage()
{
    LOG(LEVEL::FATAL)<<"命令行参数输入错误"<<"正确输入格式: "<<
    "[./Client xxx.x.x.x(IP) xxxx(PORT)]"<<CAGE ;
}
void GetRequestFromStdin(int* x , int* y , char* oper)
{
    std::cout<<"[PleaseEnter(x)#]";
    std::cin >> *x ;
    std::cout<<"[PleaseEnter(y)#]";
    std::cin >> *y ;
    std::cout<<"[PleaseEnter(oper)#]";
    std::cin >> *oper ;
}
int main(int argc ,char* argv[])
{
    if(argc != 3)
    {
        Usage();
        exit(USAGE_ERR);
    }
    ENABLE_FILE_LOG_STRATEGY();
    std::shared_ptr<Socket> Sptr = std::make_shared<TcpSocket>();
    Sptr->TcpClientSocketMethod();
    InetAddr server(argv[1], argv[2]);
    Sptr->ConnectSocket(server);
    std::shared_ptr<Protocol> protocol = std::make_shared<Protocol>();
    while(true)
    {
        int x = 0 ,y =0 ;
        char oper = '0';
        GetRequestFromStdin(&x , &y , &oper);
        std::string RequestString = 
            protocol->BulidRequestString(x,y,oper);
        Sptr->SendMessage(&RequestString);
        protocol->GetRespone(Sptr);
    }
    Sptr->Close();
    return 0;
}
