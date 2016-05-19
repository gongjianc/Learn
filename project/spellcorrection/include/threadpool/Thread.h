#ifndef __MY_THREAD_H__
#define __MY_THREAD_H__

#include "Noncopyable.h"
#include <pthread.h>
#include <functional>
#include "Cache.h"

namespace wd{

class Thread : Noncopyable{
public:
    typedef std::function<void(Cache &)> ThreadCallback;
    Thread(ThreadCallback cb, Cache &cache);
    ~Thread();

    void start();
    void join();

    ::Cache &getCache()
    { return _cache; }
    static void *threadFunc(void *arg);
private:
    pthread_t _pthId;
    bool _isRunning;
    ThreadCallback _cb;
    Cache _cache;
};

}

#endif
