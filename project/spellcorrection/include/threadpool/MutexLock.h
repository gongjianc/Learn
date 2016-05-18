#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__

#include "Noncopyable.h"
#include <pthread.h>

namespace wd{
class MutexLock : Noncopyable{
public:
    MutexLock();
    ~MutexLock();

    void lock();
    void unlock();
    //所做为何
    pthread_mutex_t *getMutexLockPtr();
private:
    pthread_mutex_t _mutex;
    bool _isLocked;
};

class MutexLockGuard{
public:
    MutexLockGuard(MutexLock &mutex)
        : _mutex(mutex)
    {
        _mutex.lock();
    }
    ~MutexLockGuard()
    {
        _mutex.unlock();
    }
private:
    MutexLock &_mutex;
};

}

#endif
