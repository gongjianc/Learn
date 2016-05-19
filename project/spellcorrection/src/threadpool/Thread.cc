/*
** @file    Thread.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-18 16:23:38
*/
 
#include "Thread.h"
#include <iostream>

namespace wd{

Thread::Thread(ThreadCallback cb, Cache &cache)
//为什么是0?
: _pthId(0)
, _isRunning(false)
, _cb(cb)
, _cache(cache)
{}

Thread::~Thread()
{
    if(_isRunning)
        pthread_detach(_pthId);
}

void Thread::start()
{
    pthread_create(&_pthId, NULL, &Thread::threadFunc, this);
    _isRunning = true;
}

void Thread::join()
{
    if(_isRunning)
        pthread_join(_pthId, NULL);
}

void * Thread::threadFunc(void *arg)
{
    Thread *pThread = static_cast<Thread *>(arg);
    if(pThread)
        pThread->_cb(pThread->_cache);
}

}
