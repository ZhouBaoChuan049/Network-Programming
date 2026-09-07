#ifndef __HTTP__
#define __HTTP__

#include "./BasicElement/Common.hpp"
#include "TcpServer.hpp"
#include "Util.hpp"
const std::string LINE_SEP = " ";
const std::string NEW_LINE = "\r\n";
const std::string BLANK_LINE = "\r\n\r\n";
const std::string KV_SEP = ": ";

const std::string WEB_ROOT = "./wwwroot";
const std::string HOME_PAGE = "/homepage.html";
const std::string NOFOUND_PAGE = "/404.html";
const std::string LOGIN_PAGE = "/login.html";
const std::string REGISTER_PAGE = "/register.html";

const std::string ImageOne = "/image/first.jpg";
const std::string ImageTwo = "/image/second.png";
const std::string ImageThree = "/image/third.png";
using namespace TcpServerModule;
using namespace HttpUtilModule;
namespace HttpModule
{
    class HttpRequest
    {
    private:
        std::string GetOneElement(std::string &RequestString)
        {
            int pos = RequestString.find(LINE_SEP);
            std::string ret = RequestString.substr(0, pos);
            RequestString.erase(0, pos - 0 + LINE_SEP.size());
            return ret;
        }
        void GetRequestLine(std::string &RequestString)
        {
            _Method = GetOneElement(RequestString);
            _Uri = GetOneElement(RequestString);
            //_Version = GetOneElement(RequestString);
            //Version单独处理
            int pos = RequestString.find(NEW_LINE);
            _Version = RequestString.substr(0, pos);
            RequestString.erase(0, pos + NEW_LINE.size());
        }
        void GetRequestHead(std::string &RequestHead)
        {
            while (true)
            {
                int pos_1 = RequestHead.find(KV_SEP);
                int pos_2 = RequestHead.find(NEW_LINE);
                if (pos_1 == std::string::npos || pos_2 == std::string::npos)
                    break;
                std::string key = RequestHead.substr(0, pos_1);
                std::string value = RequestHead.substr(pos_1 + KV_SEP.size(),
                                                       pos_2 - pos_1 - KV_SEP.size());
                _Head.insert(std::make_pair(key, value));
                RequestHead.erase(0, pos_2 + NEW_LINE.size());
            }
        }
    public:
        HttpRequest()
        {}
        void Deserialize(std::string &RequestString)
        {
            GetRequestLine(RequestString);
            _BlankLine = BLANK_LINE;
            int pos = RequestString.find(_BlankLine);
            std::string RequestHead = RequestString.substr(0, pos - 0 + NEW_LINE.size());
            GetRequestHead(RequestHead);
            RequestString.erase(0, pos - 0 + NEW_LINE.size() + _BlankLine.size());
            _Text = RequestString;
        }
        ~HttpRequest()
        {}
        std::string GetMethod() { return _Method; }
        std::string GetUri() { return _Uri; }
    private:
        std::string _Method;
        std::string _Uri;
        std::string _Version;
        std::unordered_map<std::string, std::string> _Head;
        std::string _BlankLine;
        std::string _Text;
    };
    class HttpResponse
    {
    private:
        bool FileIsExist(std::string Uri)
        {
            for (auto e : _FileSource)
                if (e == Uri)
                    return true;
            return false ;
        }
    public:
        HttpResponse()
        {}
        ~HttpResponse()
        {}
        void CheckFileSource()
        {
            _FileSource.insert("/");
            _FileSource.insert(HOME_PAGE);
            _FileSource.insert(NOFOUND_PAGE);
            _FileSource.insert(LOGIN_PAGE);
            _FileSource.insert(REGISTER_PAGE);
            _FileSource.insert(ImageOne);
            _FileSource.insert(ImageTwo);
            _FileSource.insert(ImageThree);
        }
        void BuildHeader(std::string key, std::string value)
        {
            _Head.insert({key, value});
        }
        std::string GetSuffix(std::string Uri)
        {
            int pos = Uri.rfind('.');
            if (pos == std::string::npos)
                return "NoneSuffix";
            return Uri.substr(pos);
        }
        std::string BuildResponse(std::string FileName, std::string Uri)
        {
            CheckFileSource();
            _BlankLine = BLANK_LINE;
            bool IsExist = FileIsExist(Uri);
            _Version = "HTTP/1.1";
            if (!IsExist) 
            {
                _Code = "302";
                _Dict = "NoFound";
                BuildHeader("Location", NOFOUND_PAGE);
                BuildHeader("Content-Length", "0");
                _Text = "";
            }
            else 
            {
                _Code = "200";
                _Dict = "OK";
                Util util;
                long size = util.GetFileSize(FileName);
                BuildHeader("Content-Length", std::to_string(size));
                std::string suffix = GetSuffix(Uri);
                if (suffix == ".jpg")
                    BuildHeader("Content-Type", "image/jpeg");
                else if (suffix == ".png") //
                    BuildHeader("Content-Type", "image/png");
                else if (suffix == ".html") //
                    BuildHeader("Content-Type", "text/html");
                util.GetFileContent(FileName ,_Text);
            }
            std::string ResponseString;
            ResponseString += _Version;
            ResponseString += LINE_SEP;
            ResponseString += _Code;
            ResponseString += LINE_SEP;
            ResponseString += _Dict;
            ResponseString += NEW_LINE;
            for (auto e : _Head)
            {
                ResponseString += e.first;
                ResponseString += KV_SEP;
                ResponseString += e.second;
                ResponseString += NEW_LINE;
            }
            ResponseString += _BlankLine;
            ResponseString += _Text;
            return ResponseString ;
        }
    private:
        std::string _Version;
        std::string _Code;
        std::string _Dict;
        std::unordered_map<std::string, std::string> _Head;
        std::string _BlankLine;
        std::string _Text;

        std::unordered_set<std::string> _FileSource;
    };

    class Http
    {
    public:
        void GetRequest(std::shared_ptr<Socket> Sock)
        {
            std::string Message;
            Message.resize(4096);
            int n = Sock->RecvMessage(&Message);
            if (n > 0)
            {
                HttpRequest Request;
                Request.Deserialize(Message);
                std::string Uri = Request.GetUri();
                std::string FileName;
                if (Uri == "/")
                    FileName = WEB_ROOT + HOME_PAGE;
                else if (Uri != "/")
                    FileName = WEB_ROOT + Uri;
                HttpResponse response;
                std::string ResponseString = response.BuildResponse(FileName, Uri);
                Sock->SendMessage(&ResponseString);
            }
            else if (n < 0)
                LOG(LEVEL::ERROR) << "读取浏览器HTTP请求失败" << CAGE;
        }
        void Start(char *port)
        {
            std::shared_ptr<TcpServer> server =
                std::make_shared<TcpServer>(port, [this](std::shared_ptr<Socket> Sock)
                                            { this->GetRequest(Sock); });
            server->Start();
        }
    private:
    };
}
#endif