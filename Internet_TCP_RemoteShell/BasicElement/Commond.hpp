#ifndef __COMMOND__
#define __COMMOND__
#include "Common.hpp"
#include "Log.hpp"
#include "InetAddr.hpp"
using namespace LogModule ;
using namespace InetAddrModule ;
namespace CommondModule
{
    class Commond
    {
    public:
        Commond()
        {
            _WriteTable.push_back("ll");
            _WriteTable.push_back("who");
            _WriteTable.push_back("whoami");
            _WriteTable.push_back("pwd");
            _WriteTable.push_back("ls");
            _WriteTable.push_back("ls -l");
            _WriteTable.push_back("date");
            _WriteTable.push_back("uname -a");
            _WriteTable.push_back("ps");
            _WriteTable.push_back("ps aux");
        }
        bool IsSafeCommond(std::string cmd)
        {
            auto m = std::find(_WriteTable.begin(),_WriteTable.end(),cmd);
            return m == _WriteTable.end();
        }
        std::string Excute(std::string cmd , InetAddr& user)
        {
            if(IsSafeCommond(cmd))
                LOG(LEVEL::ERROR)<<"输入的命令不存在于白名单！坏人"<<CAGE;
            FILE* fd = popen(cmd.c_str() , "r");
            char buffer[1024] ;
            std::string ans ;
            while(fgets(buffer,sizeof(buffer),fd))
                ans+=buffer ;
            ans += user.StringInfo();
            pclose(fd);
            return ans ;
        }
        ~Commond()
        {}
    private:
        std::vector<std::string> _WriteTable;
    };
}

#endif