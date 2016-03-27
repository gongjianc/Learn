#ifndef __MY_THREAD_H__
#define __MY_THREAD_H__

#include <pthread.h>

class Thread{
public:
    Thread();
    virtual ~Thread();

    void start();
    void join();
    //
protected:
    virtual void run() = 0;
private:
    //默认的this指针
    //为什么
    static void *threadFunc(void *);
private:
    //
    pthread_t _pthID;
    bool _isRunning;
};

#endif
