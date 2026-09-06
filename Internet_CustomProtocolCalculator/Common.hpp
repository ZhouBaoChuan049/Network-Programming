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

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <signal.h>
#include <jsoncpp/json/json.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

class NoCopy
{
public:
    NoCopy() {}
    ~NoCopy() {}
    const NoCopy &operator=(const NoCopy &ncpy) = delete;
    NoCopy(const NoCopy &ncpy) = delete;
};

enum BUGINFO
{
    OK = 0,
    USAGE_ERR,
    SOCKET_ERR,
    BIND_ERR,
    LISTEN_ERR,
    ACCEPT_ERR,
    CHILD_ERR,
    WAIT_ERR,
    CONNECT__ERR,
    OPEN_ERR
};

#endif