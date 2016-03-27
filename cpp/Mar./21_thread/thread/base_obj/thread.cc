/*
** @file    thread.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-20 23:41:50
*/
 
#include "thread.h"
#include <iostream>
 
Thread::Thread(ThreadCallback cb)
: _pthID(0)
, _isRunning(false)
, _cb(cb)
{
    std::cout << "Thread()" << std::endl;
}
void Thread::start()
{
    pthread_create(&_pthID, NULL, threadFunc, this);
    _isRunning = true;
}

void *Thread::threadFunc(void *arg)
{
    Thread *pThread = static_cast<Thread *>(arg);
    if(pThread != NULL){
        pThread->_cb();
        return pThread;
    }
    return NULL;
}

void Thread::join()
{
    pthread_join(_pthID, NULL);
}

Thread::~Thread()
{
    std::cout << "~Thread()" << std::endl;
    if(_isRunning)
        pthread_detach(_pthID);//交给系统去管理
}
