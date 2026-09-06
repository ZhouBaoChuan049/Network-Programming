#ifndef __MUTEXMODULE__
#define __MUTEXMODULE__
#include "Common.hpp"
namespace MutexModule
{
    class Mutex
    {
    public:
        Mutex()
        {
            int n = pthread_mutex_init(&_mutex, nullptr);
            (void)n;
        }
        void Lock()
        {
            int n = pthread_mutex_lock(&_mutex);
            (void)n;
        }
        void UnLock()
        {
            int n = pthread_mutex_unlock(&_mutex);
            (void)n;
        }
        ~Mutex()
        {
            int n = pthread_mutex_destroy(&_mutex);
            (void)n;
        }
        pthread_mutex_t* Get()
        {
            return &_mutex;
        }
    
    private:
        pthread_mutex_t _mutex;
    };
    class MutexGuard
    {
    public:
        MutexGuard(Mutex& mutex)
            : _mutex(mutex)
        {
            _mutex.Lock();
        }
        ~MutexGuard()
        {
            _mutex.UnLock();
        }

    private:
        Mutex& _mutex;
    };
}
#endif