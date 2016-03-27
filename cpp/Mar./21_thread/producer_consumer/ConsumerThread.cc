/*
** @file    ConsumerThread.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-21 05:47:51
*/
 
#include "ConsumerThread.h"
#include "Buffer.h"
#include <iostream>
#include <unistd.h>

namespace wd{

ConsumerThread::ConsumerThread(Buffer &buf)
: _buf(buf)
{}

void ConsumerThread::run()
{
    while(1)
    {
        int num = _buf.pop();
        std::cout << "Consume a number: " << num << std::endl;
        sleep(2);
    }
}


}//end of namespace
