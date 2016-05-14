/*
** @file    test5.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-04-19 17:19:50
*/
 
#include <iostream>
#include <string.h>
 
using std::cout;
using std::endl;

void fun1(char a1[3])
{
    int c1 = sizeof(a1);
    cout << "c1 = " << c1 << endl;
    cout << "strlen(a) = " << strlen(a1) << endl;
}

void fun2(char a2[])
{
    int c2 = sizeof(a2);
    cout << "c2 = " << c2 << endl;
    cout << "strlen(a) = " << strlen(a2) << endl;
}

int main(void)
{
    char a[3] = {'a', 'b', 'c'};
    int b = 2, c = 2;
    //c++ = b - c;
    cout << c << endl;
    cout << "======" << endl;
    fun1(a);
    fun2(a);
    return 0;
}

