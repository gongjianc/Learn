/*
** @file    ThreadPool.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-18 16:08:58
*/

#include "ThreadPool.h"
#include "Thread.h"
#include <unistd.h>
#include <iostream>

namespace wd{

ThreadPool::ThreadPool(int threadNum, int bufSize, MyConf &conf)
: _threadNum(threadNum)
, _bufSize(bufSize)
, _buf(_bufSize)
, _isExit(false)
, _conf(conf)
{
    _vecThreads.reserve(_threadNum);
}

ThreadPool::~ThreadPool()
{
    if(!_isExit)
        stop();
}

void ThreadPool::start()
{
    readCache();
    for(int idx = 0; idx != _threadNum; ++idx){
        Thread *pThread = new Thread(std::bind(&ThreadPool::threadFunc, this, std::placeholders::_1), _cache);
        _vecThreads.push_back(pThread);
    }
    std::vector<Thread *>::iterator it = _vecThreads.begin();
    for(; it != _vecThreads.end(); ++it)
        (*it)->start();
}

void ThreadPool::stop()
{
    if(!_isExit){
        while(!_buf.empty()){
            sleep(1);
        }
        _isExit = true;
        std::vector<Thread *>::iterator it;
        for(it = _vecThreads.begin(); it != _vecThreads.end(); ++it){
            (*it)->join();
            //？
            delete (*it);
        }
        _vecThreads.clear();
    }
}

void ThreadPool::addTask(Task ptask)
{
    _buf.push(ptask);
}

ThreadPool::Task ThreadPool::getTask()
{
    return _buf.pop();
}

void ThreadPool::threadFunc(Cache &cache)
{
    while(!_isExit){
        Task ptask = getTask();
        if(ptask)
            ptask(cache);
    }
}

void ThreadPool::readCache()
{
    _cache.readFromFile(_conf.getMap()["my_cache"]);
}

void ThreadPool::updateCache()
{
    std::cout << "time executing, update Caches" << std::endl;
    for(int idx = 0; idx != _threadNum; ++idx)
        _cache.update(_vecThreads[idx]->getCache());
    for(int idx = 0; idx != _threadNum; ++idx)
        _vecThreads[idx]->getCache().update(_cache);
    _cache.writeToFile(_conf.getMap()["my_cache"]);
}

}
