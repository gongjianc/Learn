/*
** @file    test1.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-04-09 16:00:15
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

int i = 1;

int main(void)
{
    int i = ::i;
    cout << i << endl;
    return 0;
}

