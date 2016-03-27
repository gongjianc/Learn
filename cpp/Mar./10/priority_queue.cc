/*
** @file    priority_queue.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-09 23:07:16
*/
 
#include <iostream>
#include <queue>
 
using std::cout;
using std::endl;
using std::priority_queue;

int main(void)
{
    int arr[10] = {0, 1, 3, 2, 5, 6, 9, 8, 7, 4};
    //优先级队列的底层实现是堆排序
    priority_queue<int> pqueInt;

    for(int idx = 0; idx != 10; ++idx){
        pqueInt.push(arr[idx]);
        cout << pqueInt.top() << "is the highest." << endl;
    }
    cout << "priority_queue.size = " << pqueInt.size() << endl;

    while(pqueInt.size() > 0){
        cout << pqueInt.top() << endl;
        pqueInt.pop();
    }
    cout << "priority_queue.size = " << pqueInt.size() << endl;
    return 0;
}
