/*
** @file    ProducerThread.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-21 05:43:04
*/
 
#include "ProducerThread.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "Buffer.h"
#include <iostream>

namespace wd{

ProducerThread::ProducerThread(Buffer &buf)
: _buf(buf)
{}

void ProducerThread::run()
{
    
    while(1){
        ::srand(::time(NULL));
        int num = ::rand() % 100;
        _buf.push(num);
        std::cout << "produce a number : " << num << std::endl;
        sleep(1);
    }
}

}//end of namespace 
