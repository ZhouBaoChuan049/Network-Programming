#ifndef __DIRECTORY__
#define __DIRECTORY__
#include "Log.hpp"
using namespace LogModule;
namespace DictionaryModule
{
    const std::string DEFAULT_PATH = "./dict";
    const std::string DEFAULT_NAME = "dict.txt";
    std::string SEP = ": ";
    class Dictionary
    {
    public:
        Dictionary(std::string path = DEFAULT_PATH, std::string name = DEFAULT_NAME)
            : _path(path),
              _name(name)
        {
            _pathname = _path + ((_path.back() ==
                                '/'
                            ? '/'
                            : '/') + _name);
        }
        // void DEBUGPRINTF()
        // {
        //     std::cout<<"###############################"<<std::endl;
        //     for(auto &e : _dict)
        //     {
        //         std::cout<<e.first<<e.second<<std::endl;
        //     }
        // }
        bool LoadDict()
        {
            std::string line;
            std ::ifstream ifs;
            ifs.open(_pathname);
            if(!ifs.is_open())
            {
                LOG(LEVEL::ERROR)<<"file open fail"<<CAGE;
                return false ;
            }
            while (getline(ifs, line))
            {
                size_t pos = line.find(SEP);
                if(pos == std::string ::npos)
                {
                    LOG(LEVEL::ERROR)<<"解析失败"<<CAGE;
                    continue ;
                }
                std::string english = line.substr(0, pos);
                std::string chinese = line.substr(pos + SEP.size());
                if(english.empty() || chinese.empty())
                {
                    LOG(LEVEL::ERROR)<<"不存在有效内容"<<CAGE ;
                    continue ;
                }
                _dict.insert(std::make_pair(english, chinese));
                LOG(LEVEL::INFO)<<"加载:"<<"["<<english<<": "
                <<chinese<<"成功"<<CAGE;
            }
            //DEBUGPRINTF();
            ifs.close();
            return true ;
        }
        std::string Translate(std::string engilsh)
        {
            auto it = _dict.find(engilsh);
            if (it != _dict.end())
                return it->second;
            else
                return "None (no found)";
        }

    private:
        std::string _path;
        std::string _name;
        std::string _pathname;
        std::unordered_map<std::string, std::string> _dict;
    };
}
#endif