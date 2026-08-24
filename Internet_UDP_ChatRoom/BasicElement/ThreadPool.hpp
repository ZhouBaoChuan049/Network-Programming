#ifndef __THREAD_POOL__
#define __THREAD_POOL__
#include "Common.hpp"
#include "Log.hpp"
#include "Thread.hpp"
#include "Cond.hpp"

namespace ThreadPoolModuleBasedOnSingletonPattern
{
    using namespace LogModule;
    using namespace MutexModule;
    using namespace ConModule;
    using namespace ThreadModule;

    static const size_t DEFULT_SIZE = 5;
    template <typename T>
    class ThreadPool
    {
    private:
        void WeakUpAllThread()
        {
            if (_ThreadSleepSize)
                _cond.Broadcast();
            LOG(LEVEL::INFO) << "唤醒所有休眠的线程" << CAGE;
        }
        void WeakUpOneThread()
        {
            _cond.Signal();
            LOG(LEVEL::INFO) << "唤醒一个休眠线程" << CAGE;
        }
        ThreadPool()
            : _ThreadSleepSize(0),
              _isrunning(false),
              _Size(DEFULT_SIZE)
        {
            for (int i = 0; i < DEFULT_SIZE; i++)
            {
                _Thread.emplace_back(
                    [this]()
                    {
                        HanderTask();
                    });
            }
        }
        void Start()
        {
            if (_isrunning)
                return;
            _isrunning = true;
            for (auto &e : _Thread)
            {
                e.Start();
                LOG(LEVEL::INFO) << "创建线程池成功" << e.Name();
            }
        }
        ThreadPool<T>(const ThreadPool<T> &threadpool) = delete;
        ThreadPool<T> &operator=(const ThreadPool<T> &threadpool) = delete;

    public:
        static ThreadPool<T> *GetInstance()
        {
            if (_SingPtr == nullptr)
            {
                MutexGuard mutexguard(_lock);
                LOG(LEVEL::INFO) << "获取单例。。。" << CAGE;
                {
                    if (_SingPtr == nullptr)
                    {
                        LOG(LEVEL::INFO) << "首次使用单例，创建单例" << CAGE;
                        _SingPtr = new ThreadPool<T>;
                        _SingPtr->Start();
                    }
                }
            }
            return _SingPtr;
        }
        void HanderTask()
        {
            LOG(LEVEL::INFO) << "进入线程处理函数";
            char name[64];
            pthread_getname_np(pthread_self(), name, sizeof(name));
            while (true)
            {
                T task;
                {
                    MutexGuard mutexguard(_mutex);
                    while (_isrunning && _TaskQueue.empty())
                    {
                        LOG(LEVEL::DEBUG) << "进入等待";
                        _ThreadSleepSize++;
                        _cond.Wait(_mutex);
                        _ThreadSleepSize--;
                        LOG(LEVEL::DEBUG) << "被唤醒";
                    }
                    LOG(LEVEL::INFO)
                        << "退出等待 queue="
                        << _TaskQueue.size();
                    if (!_isrunning && _TaskQueue.empty())
                    {
                        LOG(LEVEL::INFO) << "线程池退出&7任务队列为空" << CAGE;
                        break;
                    }
                    task = _TaskQueue.front();
                    LOG(LEVEL::INFO) << "取出任务";
                    _TaskQueue.pop();
                }
                task();
            }
        }
        bool Equeue(const T &task)
        {
            if (_isrunning)
            {
                MutexGuard mutexguard(_mutex);
                _TaskQueue.push(task);
                if (_ThreadSleepSize > 0)
                    WeakUpOneThread();
                return true;
            }
            return false;
        }
        void Stop()
        {
            LOG(LEVEL::DEBUG)<<"线程池停止"<<CAGE;
            if (_isrunning)
            {
                MutexGuard mutexguard(_mutex);
                _isrunning = false;
                if (_ThreadSleepSize)
                {
                    WeakUpAllThread();
                }
            }
        }
        void Join()
        {
            LOG(LEVEL::DEBUG)<<"线程池回收线程"<<CAGE;
            if (!_isrunning)
            {
                for (auto &e : _Thread)
                {
                    e.join();
                }
            }
        }
    private:
        size_t _ThreadSleepSize;
        bool _isrunning;
        std::vector<Thread> _Thread;
        std::queue<T> _TaskQueue;
        size_t _Size; // 线程个数
        Mutex _mutex;
        Cond _cond;
        static ThreadPool<T> *_SingPtr;
        static Mutex _lock;
    };
    template <class T>
    ThreadPool<T> *ThreadPool<T>::_SingPtr = nullptr;
    template <class T>
    Mutex ThreadPool<T>::_lock;
}
#endif