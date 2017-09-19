/*
** @file    move.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-16 04:27:08
*/
 
#include <iostream>
#include <utility>
 
using std::cout;
using std::endl;

int main(void)
{
    int &&rr1 = 20;
    //int &&rr2 = rr1;
    cout << rr1 << endl;

    int &&rr2 = std::move(rr1);
    cout << rr2 << endl;

    cout << rr1 << endl;
    cout << rr2 << endl;
    return 0;
}
