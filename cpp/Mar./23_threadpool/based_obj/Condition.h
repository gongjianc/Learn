#ifndef __WD_CONDITION_H__
#define __WD_CONDITION_H__

#include "Noncopyable.h"
#include <pthread.h>
#include "MutexLock.h"

namespace wd{

class Condition : Noncopyable{
public:
    Condition(MutexLock &mutex);
    ~Condition();

    void wait();
    void notify();
    void notifyall();
private:
    pthread_cond_t _cond;
    MutexLock &_mutex;//为什么是引用类型？
};

}//end of namespace

#endif
