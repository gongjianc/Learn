/*
** @file    MutexLock.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-21 04:53:43
*/
 
#include "MutexLock.h"
#include <iostream>
 
using std::cout;
using std::endl;

namespace wd{

MutexLock::MutexLock()
: _isLocked(false)
{
    pthread_mutex_init(&_mutex, NULL);
}

MutexLock::~MutexLock()
{
    if(!_isLocked)
        pthread_mutex_destroy(&_mutex);
}

void MutexLock::lock()
{
    pthread_mutex_lock(&_mutex);
    _isLocked = true;
}

void MutexLock::unlock()
{
    pthread_mutex_unlock(&_mutex);
}

pthread_mutex_t *MutexLock::getMutexLockPtr()
{
    return &_mutex;
}


}//end of naamespace
