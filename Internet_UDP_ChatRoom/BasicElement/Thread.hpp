#include "Mutex.hpp"
#include "Log.hpp"
#include "Common.hpp"
#include "InetAddr.hpp"
using namespace InetAddrModule ;
namespace ThreadModule
{
    int count = 1 ;
    class Thread
    {
    private:
    static void* Routine(void* args)
    {
        Thread* self = static_cast<Thread*>(args); 
        pthread_setname_np(pthread_self(),self->_name);
        if(!self->_isrunning)
            self->_isrunning = true ;
        self->_func();//转化成了this->_func();
        return nullptr ;
    } 
    public:
        Thread(task_t task)
            :_tid(0),
            _func(task),
            _isrunning(false)
        {
            snprintf(_name , sizeof(_name),"Thread-%d",count);
            count++ ;
        }
        void Start()
        {
            if(_isrunning)
            {
                return ;
            }
            int n = pthread_create(&_tid,nullptr,&Routine , this);
            if(n == 0) LogModule::LOG(LogModule::LEVEL::DEBUG)<<"线程创建成功"<<LogModule::CAGE; 
            else LogModule::LOG(LogModule::LEVEL::ERROR)<<"线程闯进啊失败"<<LogModule::CAGE;
        }
        void join()
        {
            pthread_join(_tid,nullptr);
        }
        char* Name()
        {
            return _name ;
        }
    private:
        pthread_t _tid ;
        bool _isrunning ;
        task_t _func ;
        char _name[128] ;
    };
} 