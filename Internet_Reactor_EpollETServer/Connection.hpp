#ifndef __CONNECTION__
#define __CONNECTION__

#include "Common.hpp"
namespace ReactorModule
{
    class Reactor ;
}

using _CallBackFunc = std::function<std::string(std::string*)> ;
namespace ConnectionModule
{
    class Connection
    {
    public:
        Connection()
        {}
        void SetCallBackFunc(_CallBackFunc func)
        {
            _func = func ;
        }
        virtual int Getsocketfd() = 0;
        virtual void Recv() = 0;
        virtual void Send() = 0;
        uint32_t GetEvent()
        {
            return _event ;
        }
        void SetEvent(uint32_t event)
        {
            _event = event;
        }
        void SetOwner(ReactorModule::Reactor* owner)
        {
            _owner = owner ;
        }
        ReactorModule::Reactor* GetOwner()
        {
            return _owner;
        }
        ~Connection()
        {}
    private:
        uint32_t _event;
        ReactorModule::Reactor* _owner ;
    public:
        _CallBackFunc _func ;
    };
}


#endif