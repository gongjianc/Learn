/*
** @file    TimerThread.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-19 16:47:56
*/


#include "TimerThread.h"

TimerThread::TimerThread(TimerThreadCallback cb, 
        int init_sec, int interval_sec)
: _timer(cb, init_sec, interval_sec)
//thread的_pthID, _isRunning都有默认参数，只需要传递一个 cb 过去
, _thread(std::bind(&Timer::start, &_timer))
{}

void TimerThread::start()
{
    _thread.start();
}

void TimerThread::stop()
{
    _timer.stop();
}
