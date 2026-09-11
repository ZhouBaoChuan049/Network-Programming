#ifndef __CHANNEL__
#define __CHANNEL__

#include "Common.hpp"
#include "Connection.hpp"
#include "Reactor.hpp"
using namespace ConnectionModule ;
using namespace ReactorModule ;
namespace ChannelModule
{
const int MAX_RECVBUFFER = 4096; 
    class Channel :public Connection
    {
    public:
        Channel(int sockfd)        
            :_sockfd(std::make_unique<TcpSocket>(sockfd))
        {}
        ~Channel()
        {}
        int Getsocketfd() override
        {
            return _sockfd->GetSockFd() ;
        }
        void Recv() override
        {
            while(true)
            {
                std::string RecvBuffer(MAX_RECVBUFFER , '\0') ;
                int n = recv(_sockfd->GetSockFd(), RecvBuffer.data(), sizeof(RecvBuffer), 0);
                if(n > 0)
                {
                    RecvBuffer.resize(n);
                    _inbuffer += RecvBuffer ;
                    LOG(LEVEL::INFO)<<"读取数据成功"<<CAGE;
                }
                else if(n == 0)
                {
                    LOG(LEVEL::INFO)<<"对端已关闭连接"<<CAGE;
                    Exceptor() ;
                    break;
                }
                else if(errno == EAGAIN || errno == EWOULDBLOCK)
                    break ;
                else if(errno == EINTR)
                    continue ;
                else    
                {
                    LOG(LEVEL::ERROR)<<"异常错误!错误原因:接收请求失败."<<CAGE;
                    Exceptor() ;
                }
            }
            if(_outbuffer.empty())
            {
                LOG(LEVEL::DEBUG)<<"将要执行 回调处理"<<CAGE;
                if(_func != nullptr)
                    _outbuffer += _func(&_inbuffer) ;
                LOG(LEVEL::DEBUG)<<"回调处理结束"<<CAGE;
                //_inbuffer.clear();
            }
            if(!_outbuffer.empty())
                Send();
        }
        void Send() override
        {
            while(true)
            {
                int n = send(_sockfd->GetSockFd() , _outbuffer.data() , _outbuffer.size(), 0);
                if(n > 0)
                {
                    _outbuffer.erase(0, n);
                    LOG(LEVEL::INFO)<<"发送应答成功,本轮发送数据:"<<n<<CAGE;
                    if(_outbuffer.empty())
                        break;
                }
                else if(errno == EAGAIN || errno == EWOULDBLOCK)
                    break ;
                else if(errno == EINTR)
                    continue ;
                else
                {
                    LOG(LEVEL::ERROR)<<"异常错误!错误原因:发送请求失败."<<CAGE;
                    Exceptor() ;
                }
            }
            if(!_outbuffer.empty())
            {
                LOG(LEVEL::DEBUG)<<"修改"<<CAGE;
                GetOwner()->TakeCareOfInOut(_sockfd->GetSockFd() , EPOLLIN , EPOLLOUT);
            }
            else if(_outbuffer.empty())
            {
                LOG(LEVEL::DEBUG)<<"恢复"<<CAGE;
                GetOwner()->TakeCareOfInOut(_sockfd->GetSockFd() , EPOLLIN , 0);
            }
        }
        void Exceptor()
        {
            GetOwner()->DelConnection(_sockfd->GetSockFd());
        }
    private:
        std::unique_ptr<TcpSocket> _sockfd ;
        std::string _inbuffer ;
        std::string _outbuffer ;
    };
}

#endif