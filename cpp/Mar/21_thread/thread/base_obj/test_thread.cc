/*
** @file    test_thread.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-20 23:58:42
*/
 
#include "thread.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <memory>
#include <functional>
 
using std::cout;
using std::endl;

void run()
{
    int i = 10;
    while(i--){
        ::srand(::time(NULL));
        int num = rand() % 100;
        cout << "num = " << num << endl;
        ::sleep(1);//为什么用::运算符
    }
}

int main(void)
{
    Thread *pThread = new Thread(run);
    std::unique_ptr<Thread> threadPtr(pThread);
    threadPtr->start();
    threadPtr->join();

    return 0;
}


