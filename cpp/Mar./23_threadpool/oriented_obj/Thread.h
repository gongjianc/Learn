#ifndef __MY_THREAD_H__
#define __MY_THREAD_H__

#include <pthread.h>
#include "Noncopyable.h"

namespace wd{

class Thread : Noncopyable{
public:
    Thread();
    virtual ~Thread();

    void start();
    void join();
    //
protected:
    virtual void run() = 0;
private:
    static void *threadFunc(void *);
private:
    pthread_t _pthID;
    bool _isRunning;
};//end of class Thread

}//end of namespace

#endif
