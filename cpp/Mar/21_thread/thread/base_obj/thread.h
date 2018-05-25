#ifndef __MY_THREAD_H__
#define __MY_THREAD_H__

#include <pthread.h>
#include <functional>

class Thread{
public:
    typedef std::function<void()> ThreadCallback;
    Thread(ThreadCallback cb);
    virtual ~Thread();

    void start();
    void join();
    //
private:
    static void *threadFunc(void *);
    //virtual void run() = 0;
private:
    pthread_t _pthID;
    bool _isRunning;
    ThreadCallback _cb;
};

#endif
