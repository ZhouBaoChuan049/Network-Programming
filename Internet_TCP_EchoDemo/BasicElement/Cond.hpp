#ifndef __CONDMODULE__
#define __CONDMODULE__
#include "Mutex.hpp"
#include "Common.hpp"
namespace ConModule
{
    class Cond
    {
    public:
        Cond()
        {
            int n = pthread_cond_init(&_cond, nullptr);
            (void)n;
        }
        void Wait(MutexModule::Mutex &mutex)
        {
            int n = pthread_cond_wait(&_cond, mutex.Get());
            (void)n;
        }
        void Signal()
        {
            int n = pthread_cond_signal(&_cond);
            (void)n;
        }
        void Broadcast()
        {
            int n = pthread_cond_broadcast(&_cond);
            (void)n;
        }
        ~Cond()
        {
            int n = pthread_cond_destroy(&_cond);
            (void)n;
        }

    private:
        pthread_cond_t _cond;
    };
}
#endif