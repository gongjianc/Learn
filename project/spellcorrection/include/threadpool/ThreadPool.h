#ifndef __MY_THREADPOOL_H__
#define __MY_THREADPOOL_H__

#include "Buffer.h"
#include "Cache.h"
#include "MyConf.h"
#include <vector>
#include <functional>

namespace wd{

class Thread;
class ThreadPool{
public:
    //Task 是函数对象
    typedef std::function<void(Cache &)> Task;
    ThreadPool(int threadNum, int bufSize, MyConf &conf);
    ~ThreadPool();

    void start();
    void stop();

    void addTask(Task ptask);
    void threadFunc(Cache &cache);

    void readCache();
    void updateCache();
private:
    Task getTask();
private:
    int _threadNum;
    //Num 个线程池，vector 实现
    std::vector<Thread *> _vecThreads;
    int _bufSize;
    Buffer _buf;
    bool _isExit;
    //Cache 有初始化构造函数
    Cache _cache;
    MyConf &_conf;
};
}

#endif
