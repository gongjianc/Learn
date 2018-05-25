/*
** @file    queue.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-09 23:00:17
*/
 
#include <iostream>
#include <queue>
#include <string>
#include <vector>
 
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::queue;

int main(void)
{
    queue<int> queueInt;

    for(int idx = 0; idx != 10; ++idx){
        queueInt.push(idx);
        cout << idx << "入队" << endl;
    }

    cout << "queue.size = " << queueInt.size() << endl;

    while(queueInt.size() > 0){
        cout << queueInt.front() << "出队" << endl;
        queueInt.pop();
    }
    cout << "queue.size = " << queueInt.size() << endl;

    return 0;
}
