/*
** @file    MutexLock.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-18 16:33:04
*/

#include "MutexLock.h"
#include <iostream>

namespace wd{

MutexLock::MutexLock()
: _isLocked(false)
{
    pthread_mutex_init(&_mutex, NULL);
}

MutexLock::~MutexLock()
{
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
    _isLocked = false;
}

pthread_mutex_t *MutexLock::getMutexLockPtr()
{
    return &_mutex;
}

}
