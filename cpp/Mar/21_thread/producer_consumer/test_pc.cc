/*
** @file    test_pc.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-21 05:51:42
*/
 
#include "ProducerThread.h"
#include "ConsumerThread.h"
#include "Buffer.h"
#include <iostream>
#include <memory>
using std::cout;
using std::endl;

using namespace wd;

int main(void)
{
    Buffer buffer(10);
    Thread *pProducter = new ProducerThread(buffer);
    Thread *pConsumer = new ConsumerThread(buffer);

    std::unique_ptr<Thread> producter(pProducter);
    std::unique_ptr<Thread> consumer(pConsumer);

    producter->start();
    consumer->start();

    producter->join();
    consumer->join();

    return 0;

}
