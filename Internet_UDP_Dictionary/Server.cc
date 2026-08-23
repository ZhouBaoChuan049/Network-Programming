#include "Log.hpp"
#include "Server.hpp"
#include "Dictionary.hpp"
using namespace DictionaryModule ;
using namespace DictionaryServer ;
using namespace LogModule ;
int main(int argc , char* argv[])
{
    if(argc != 2 )
    {
        LOG(LEVEL::FATAL)<<"输入错误，因该输入./Server 端口号"
        <<CAGE ;
        return 1 ;
    }
    // ./Server 8080
    ENABLE_FILE_LOG_STRATEGY() ;
    uint16_t local_port = std::stoi(argv[1]);
    Dictionary dict ;
    dict.LoadDict() ;
    DictServer dictserver(htons(local_port) ,[&dict](std::string english)->std::string{
        return dict.Translate(english);
    }) ;
    dictserver.Init();
    dictserver.Start();
    return 0 ;
}