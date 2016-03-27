#ifndef __WD_THREADPOOLTHREAD_H__
#define __WD_THREADPOOLTHREAD_H__

#include "Thread.h"
//#include "Threadpool.h"

namespace wd{

class Threadpool;
class ThreadpoolThread : public Thread{
public:
    ThreadpoolThread(Threadpool &threadpool);
    
    void run();
private:
    Threadpool &_threadpool;
};
}//end of namespace

#endif
