#ifndef __WD_BUFFER_H__
#define __WD_BUFFER_H__

#include "Noncopyable.h"
#include "MutexLock.h"
#include "Condition.h"
#include <queue>

namespace wd{

class Buffer : Noncopyable{
public:
    Buffer(std::size_t size);

    void push(int);
    int pop();
    bool full();
    bool empty();
private:
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
    std::size_t _size;
    std::queue<int> _que;
};

}// end of namespace wd

#endif
