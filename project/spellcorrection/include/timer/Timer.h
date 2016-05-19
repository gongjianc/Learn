#ifndef __MY_TIMER_H__
#define __MY_TIMER_H__

#include "Noncopyable.h"
#include <iostream>
#include <functional>

class Timer : wd::Noncopyable{
public:
    typedef std::function<void()> TimerCallback;
    Timer(TimerCallback cb, int init_sec, int interval_sec);
    ~Timer();

    void start();
    void stop();
private:
    int _fd;
    TimerCallback _cb;
    bool _isStarted;
};

#endif
