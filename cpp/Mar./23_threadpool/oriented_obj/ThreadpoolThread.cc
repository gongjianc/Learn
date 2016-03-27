/*
** @file    ThreadpoolThread.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-22 06:14:06
*/
 
#include "ThreadpoolThread.h"
#include "Threadpool.h"

namespace wd{

ThreadpoolThread::ThreadpoolThread(Threadpool &threadpool)
: _threadpool(threadpool)
{}

void ThreadpoolThread::run()
{
    _threadpool.threadFunc();
}

}//
