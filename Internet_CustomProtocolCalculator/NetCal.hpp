#ifndef __NET_CAL__
#define __NET_CAL__

#include "Common.hpp"
#include "Protocol.hpp"
using namespace ProtocolModule ;
namespace NetCalModule
{
    class NetCal
    {
    public:
        Respone Execute(Request& req)
        {
            Respone resp(0,0) ;
            switch(req.GetOper())
            {
                case '+':
                    resp.SetResult(req.GetX() + req.GetY());
                    break;
                case '-':
                    resp.SetResult(req.GetX() - req.GetY());
                    break;
                case '*':
                    resp.SetResult(req.GetX() * req.GetY());
                    break;
                case '/':
                {
                    if(req.GetY() == 0)
                    {
                        resp.SetCode(1);
                        LOG(LEVEL::ERROR)<<"计算出现除零错误"<<
                        "最终执行:["<<req.GetX()<<"/"<<req.GetY()
                        <<"]"<<CAGE ;
                    }
                    else
                        resp.SetResult(req.GetX() / req.GetY());
                    break;
                }
                case '%':
                {
                    if(req.GetY() == 0)
                    {
                        resp.SetCode(2);
                        LOG(LEVEL::ERROR)<<"计算出现模零错误"<<
                        "最终执行:["<<req.GetX()<<"%"<<req.GetY()
                        <<"]"<<CAGE ;
                    }
                    else
                        resp.SetResult(req.GetX() % req.GetY());
                    break;
                }
                default:
                {
                    resp.SetCode(3);
                    LOG(LEVEL::ERROR)<<"错误输入"<<CAGE;
                    break;
                }
            }
            return resp ;
        }
    };
}
#endif