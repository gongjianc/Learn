/*
** @file    test_threadpool.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-22 06:48:32
*/
 
#include <iostream>
#include "Threadpool.h"
#include "Task.h"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

using std::cout;
using std::endl;

namespace wd{
class MyTask : public Task{
public:
    void process()
    {
        ::srand(::time(NULL));
        int num = ::rand() % 100;
        cout << "produce a number : " << num << endl;
    }
};
}//end of namespace wd
 
using namespace wd;
int main(void)
{
    Threadpool threadpool(5, 10);
    threadpool.start();
    Task *pTask = new MyTask;
    
    int count = 20;
    while(count){
        threadpool.addTask(pTask);
        sleep(1);
        count--;
    }
//    threadpool.stop();
    return 0;
}

