/*
** @file    mutex.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-21 02:26:41
*/
 
#include <iostream>
#include <pthread.h>
 
using std::cout;
using std::endl;

class MutexLock{
public:
    MutexLock()
    {
        pthread_mutex_init(&_mutex, NULL);
    }
    ~MutexLock()
    {
        pthread_mutex_destroy(&_mutex);
    }

    void lock()
    {
        pthread_mutex_lock(&_mutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(&_mutex);
    }
private://禁止复制 对象语义
    MutexLock(const MutexLock &);
    MutexLock &operator=(const MutexLock &);
private:
    pthread_mutex_t _mutex;
};
