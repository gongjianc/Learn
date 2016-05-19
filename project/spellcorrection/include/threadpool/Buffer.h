#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "MutexLock.h"
#include "Condition.h"
#include "Cache.h"

#include <queue>
#include <functional>

namespace wd{

class Task;
class Buffer{
public:
    typedef std::function<void(Cache &)> Task;
    Buffer(int size);
    ~Buffer();

    void push(Task ptask);
    Task pop();

    bool full();
    bool empty();
private:
    int _size;
    std::queue<Task> _que;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
};
}

#endif
