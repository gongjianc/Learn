/*
** @file    test4.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-04-18 14:43:31
*/
 
#include <iostream>

using std::cout;
using std::endl;

int main(void)
{
    double *(* a)[3][6];
    int *c = new int[50];
    int *b[10];
    int (*d)[10];
    cout << "sizeof(b) = " << sizeof(b) << endl;
    cout << "sizeof(d) = " << sizeof(d) << endl;
    cout << sizeof(c) << endl;
    cout << sizeof(*c) << endl;
    cout << "-------" << endl;
    cout << sizeof(a) << endl;
    cout << sizeof(*a) << endl;
    cout << sizeof(**a) << endl;
    cout << sizeof(***a) << endl;
    cout << sizeof(****a) << endl;
    return 0;
}
