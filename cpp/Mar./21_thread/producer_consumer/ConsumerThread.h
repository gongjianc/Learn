#ifndef __WD_CONSUMERTHREAD_H__

#define __WD_CONSUMERTHREAD_H__

#include "Thread.h"

namespace wd{

class Buffer;

class ConsumerThread : public Thread{
public:
    ConsumerThread(Buffer &buf);
    void run();
private:
    Buffer &_buf;
};


}//end of namespace

#endif
