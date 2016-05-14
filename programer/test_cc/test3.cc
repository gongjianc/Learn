/*
** @file    test3.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-04-14 09:56:55
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

void fn(int *b)
{
    cout << (*b)++ << endl;
}

int main(void)
{
    int a = 7;
    fn(&a);
    cout << a << endl;
    return 0;
}
