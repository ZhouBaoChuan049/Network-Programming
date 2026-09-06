#ifndef __PROTOCOL__
#define __PROTOCOL__
#include "Log.hpp"
#include "Common.hpp"
#include "InetAddr.hpp"
#include "Socket.hpp"
using namespace LogModule;
using namespace InetAddrModule;
using namespace SocketModule;

namespace ProtocolModule
{
    const std::string SEP = "\r\n";
    class Request
    {
    public:
        Request(int x = 0, int y = 0, int oper = '+')
            : _x(x),
              _y(y),
              _oper(oper)
        {
        }
        std::string Serialize()
        {
            Json::Value root;
            root["x"] = _x;
            root["y"] = _y;
            root["oper"] = _oper;
            Json::FastWriter writer;
            std::string ret = writer.write(root);
            return ret; // 返回{"x":10,"y":20,"oper":'+'}
        }
        bool Deserialize(std::string JsonString)
        {
            // 传入:{"x":10,"y":20,"oper":'+'}
            Json::Value root;
            Json::Reader reader;
            bool ret = reader.parse(JsonString, root);
            _x = root["x"].asInt();
            _y = root["y"].asInt();
            _oper = root["oper"].asInt();
            return ret;
        }
        int GetX() { return _x; }
        int GetY() { return _y; }
        char GetOper() { return _oper; }

    private:
        int _x;
        int _y;
        char _oper;
    };

    class Respone
    {
    public:
        Respone(int result = 0, int code = 0)
            : _result(result),
              _code(code)
        {
        }
        std::string Serialize()
        {
            Json::Value root;
            root["result"] = _result;
            root["code"] = _code;
            Json::FastWriter writer;
            std::string ret = writer.write(root);
            return ret;
        }
        bool Deserialize(std::string JsonString)
        {
            Json::Value root;
            Json::Reader reader;
            bool ret = reader.parse(JsonString, root);
            _result = root["result"].asInt();
            _code = root["code"].asInt();
            return ret;
        }
        void SetResult(int result = 0) { _result = result; }
        void SetCode(int code = 0) { _code = code; }
        int GetResult() { return _result; }
        int GetCode() { return _code; }

    private:
        int _result;
        int _code;
    };
    using func_t = std::function<Respone(Request &)>;
    class Protocol
    {
    public:
        Protocol()
        {}
        Protocol(func_t func)
            : _work(func)
        {}
        std::string encode(std::string JsonString)
        {
            return std::to_string(JsonString.size()) + SEP + JsonString + SEP;
        }
        bool decode(std::string JsonString, std::string *request)
        {
            int pos = JsonString.find(SEP);
            if (pos == std::string::npos)
                return false;
            std::string StringLength = JsonString.substr(0, pos);
            if(StringLength.empty())
                return false ;
            for(auto e : StringLength)
                if(!(e >= '0' && e <= '9'))
                    return false ;
            int Length = stoi(StringLength);      // 报文长度
            int LengthSize = StringLength.size(); // 报文长度的长度
            if ((LengthSize + 2 * SEP.size() + Length) > JsonString.size())
                return false;
            *request = JsonString.substr(LengthSize + SEP.size(), Length);
            JsonString.erase(0, LengthSize + 2 * SEP.size() + Length);
            return true;
        }
        void GetRequest(std::shared_ptr<Socket> &sock, InetAddr &client)
        {
            while (true)
            {
                std::string RequestString;
                RequestString.resize(4096);
                int n = sock->RecvMessage(&RequestString);
                LOG(LEVEL::DEBUG)<<"RequestStringFromTheRecv :"<<RequestString<<CAGE;
                if (n == -1)
                {
                    LOG(LEVEL::ERROR) << "读取请求失败" << CAGE;
                    continue;
                }
                else if (n > 0)
                {
                    std::string request;
                    while (decode(RequestString, &request))
                    {
                        int n = _req.Deserialize(request);
                        if (n)
                            LOG(LEVEL::ERROR) << "反序列化成功" << CAGE;
                        _resp = _work(_req);
                        std::string respstring = _resp.Serialize();
                        std::string ret = encode(respstring);
                        send(sock->GetSockFd(), ret.c_str(), ret.size(), 0);
                        break;
                    }
                }
                else if (n == 0)
                {
                    LOG(LEVEL::INFO) << "客户端[" << client.GetInetIp() << "]退出" << CAGE;
                    break;
                }
            }
            LOG(LEVEL::DEBUG) << "GetRequest 即将退出" << CAGE;
        }
        void GetRespone(std::shared_ptr<Socket> &sock)
        {
            while (true)
            {
                std::string resp;
                resp.resize(4096);
                int n = sock->RecvMessage(&resp);
                if (n == -1)
                {
                    LOG(LEVEL::ERROR) << "读取响应失败" << CAGE;
                    continue;
                }
                else if (n > 0)
                {
                    std::string response;
                    while (decode(resp, &response))
                    {
                        bool m = _resp.Deserialize(response);
                        if (m)
                        {
                            LOG(LEVEL::ERROR) << "反序列化成功" << CAGE;
                            std::cout << "输出结果[ result: " << _resp.GetResult() 
                            << " code: " << _resp.GetCode() << "]" << std::endl;
                        }
                        return ;
                    }
                }
            }
        }
        std::string BulidRequestString(int x, int y, char oper)
        {
            Request request(x, y, oper);
            std::string ret = request.Serialize();
            std::string RequestString = encode(ret);
            LOG(LEVEL::DEBUG) << "构建请求串成功:" << RequestString << CAGE;
            return RequestString;
        }

    private:
        Request _req;
        Respone _resp;
        func_t _work;
    };
}

#endif