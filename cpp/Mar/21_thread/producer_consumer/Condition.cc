/*
** @file    Conditon.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-21 05:04:43
*/
 
#include "Condition.h"
#include <pthread.h>
 
namespace wd{

Condition::Condition(MutexLock &mutex)
: _mutex(mutex)
{
    pthread_cond_init(&_cond, NULL);
}

Condition::~Condition()
{
    pthread_cond_destroy(&_cond);
}

void Condition::wait()
{
    pthread_cond_wait(&_cond, _mutex.getMutexLockPtr());//为什么不是&_mutex
}

void Condition::notify()
{
    pthread_cond_signal(&_cond);
}

void Condition::notifyall()
{
    pthread_cond_broadcast(&_cond);
}

}//end of namespace wd
