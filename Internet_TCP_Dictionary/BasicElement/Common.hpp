#ifndef __COMMON__
#define __COMMON__

#include <filesystem>
#include <iostream>

#include <ctime>
#include <fstream>
#include <sstream>
#include <memory>

#include <cstdio>
#include <cstring>

#include <string>
#include <vector>
#include <queue>
#include <functional>

#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
using func_t = std::function<std::string(std::string)>;
using task_t = std::function<void()>;

class NoCopy
{
public:
    NoCopy(){}
    const NoCopy& operator=(const NoCopy& ncpy) = delete ;
    NoCopy(const NoCopy& ncpy) = delete ;
};

enum BUGINFO{
    OK = 0,
    USAGE_ERR,
    SOCKET_ERR,
    BIND_ERR,
    LISTEN_ERR,
    ACCEPT_ERR,
    CHILD_ERR,
    WAIT_ERR,
    CONNECT__ERR
};

#endif