/*
** @file    MyThread.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-19 16:35:43
*/

#include "MyThread.h"
#include <iostream>

MyThread::MyThread(ThreadCallback cb)
: _pthId(0)
, _isRunning(false)
, _cb(cb)
{}

MyThread::~MyThread()
{
    if(_isRunning)
        pthread_detach(_pthId);
}

void MyThread::start()
{
    //为什么？
    pthread_create(&_pthId, NULL, &MyThread::threadFunc, this);
    _isRunning = true;
}

void MyThread::join()
{
    if(_isRunning)
        pthread_join(_pthId, NULL);
}

void *MyThread::threadFunc(void *arg)
{
    MyThread *pThread = static_cast<MyThread *>(arg);
    if(pThread)
        pThread->_cb();
}


