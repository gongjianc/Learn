#ifndef __WD_BUFFER_H__
#define __WD_BUFFER_H__

#include "Noncopyable.h"
#include "MutexLock.h"
#include "Condition.h"
#include <queue>

namespace wd{

class Task;

class Buffer : Noncopyable{
public:
    Buffer(std::size_t size);

    void push(Task *);
    Task *pop();
    bool full();
    bool empty();
    void wakeup_wait();
private:
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
    std::size_t _size;
    std::queue<Task *> _que;
};

}// end of namespace wd

#endif
