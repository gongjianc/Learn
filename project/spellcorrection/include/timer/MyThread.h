#ifndef __MYTHREAD_H__
#define __MYTHREAD_H__

#include "Noncopyable.h"
#include <pthread.h>
#include <functional>

class MyThread : wd::Noncopyable{

public:
    typedef std::function<void()> ThreadCallback;
    MyThread(ThreadCallback cb);
    ~MyThread();

    void start();
    void join();

    static void *threadFunc(void *);
private:
    pthread_t _pthId;
    bool _isRunning;
    ThreadCallback _cb;
};

#endif
