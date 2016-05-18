#ifndef __MY_THREADPOOL_H__
#define __MY_THREADPOOL_H__

#include "Buffer.h"
#include <vector>
#include <functional>

namespace wd{

class Thread;
class ThreadPool{
public:
    typedef std::function<void()> Task;
    ThreadPool(int threadNum, int bufSize);
    ~ThreadPool();

    void start();
    void stop();
    void addTask(Task ptask);
    void threadFunc();
private:
    Task getTask();
private:
    int _threadNum;
    //Num 个线程池，vector 实现
    std::vector<Thread *> _vecThreads;
    int _bufSize;
    Buffer _buf;
    bool _isExit;
};
}

#endif
