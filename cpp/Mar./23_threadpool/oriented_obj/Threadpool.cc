/*
** @file    Threadpool.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-22 06:04:31
*/
 
#include "Threadpool.h"
#include "ThreadpoolThread.h"
#include "Task.h"

namespace wd{

Threadpool::Threadpool(size_t threadsNum, size_t bufNum)
: _threadsNum(threadsNum)
, _buf(bufNum)
, _isExit(false)
{}

void Threadpool::start()
{
    for(size_t idx = 0; idx != _threadsNum; ++idx){
       Thread *pThread = new ThreadpoolThread(*this); 
       _vecThreads.push_back(pThread);
    }
    std::vector<Thread *>::iterator it;
    for(it = _vecThreads.begin(); it != _vecThreads.end(); ++it)
        (*it)->start();
}

void Threadpool::addTask(Task *pTask)
{
    _buf.push(pTask);
}

Task *Threadpool::getTask()
{
    return _buf.pop();
}

void Threadpool::threadFunc()
{
    while(!_isExit){
        Task *pTask = getTask();
        if(pTask)
            pTask->process();
    }
}

void Threadpool::stop()
{
    if(!_isExit){
        _isExit = true;
        _buf.wakeup_wait();//告诉线程激活
        std::vector<Thread *>::iterator it;
        for(it = _vecThreads.begin(); it != _vecThreads.end(); ++it){
            (*it)->join();//回收资源
            delete (*it);
        }
        _vecThreads.clear();
    }
}

Threadpool::~Threadpool()
{
    if(!_isExit)
        stop();
}

}//end of namespace
