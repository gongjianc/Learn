#ifndef __WD_PRODUCERTHREAD_H__
#define __WD_PRODUCERTHREAD_H__

#include "Thread.h"

namespace wd{

class Buffer;

class ProducerThread : public Thread{
public:
    ProducerThread(Buffer &buf);

    void run();
private:
    Buffer &_buf;
};

}//end of namespace

#endif
