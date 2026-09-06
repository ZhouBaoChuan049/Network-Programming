#include "Common.hpp"
#include "Log.hpp"
using namespace LogModule ;
namespace DaemonModule
{
const std::string _DEV = "/dev/null" ;
    void EnableDaemon(int isnochdir , int isnoclose)
    {
        signal(SIGCLD , SIG_IGN);
        signal(SIGPIPE , SIG_IGN);
        int pid = fork();
        if(pid > 0 )
            exit(0);
        setsid();
        if(isnochdir == 0)
            chdir("/");
        if(isnoclose == 0)
        {
            int fd = ::open(_DEV.c_str() , O_RDWR);
            if(fd < 0)
            {
                LOG(LEVEL::ERROR)<<"Open The /dev/null Fail"<<CAGE ;
                exit(BUGINFO::OPEN_ERR);
            }
            else 
            {
                dup2(fd , 0);
                dup2(fd , 1);
                dup2(fd , 2);
                close(fd);
            }
        }
    }
}