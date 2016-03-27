#ifndef __WD_THREADPOOL_H__
#define __WD_THREADPOOL_H__

#include <vector>
#include "Buffer.h"

namespace wd{

class Thread;
class Threadpool{
    friend class ThreadpoolThread;
public:
    Threadpool(size_t threadsNum, size_t bufNum);
    ~Threadpool();
    void start();
    void stop();
    
    void addTask(Task *);
private:
    Task *getTask();
    void threadFunc();

private:
    std::size_t _threadsNum;
    std::vector<Thread *> _vecThreads;
    Buffer _buf;
    bool _isExit;
};
}//end of namespace wd


#endif
