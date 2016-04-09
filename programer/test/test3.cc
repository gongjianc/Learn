/*
** @file    test3.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-04-09 09:19:02
*/
 
#include <iostream>
#include <string.h>
 
using std::cout;
using std::endl;

int main(void)
{
    char a[] = "abcd";
    char *p = a;
    int b = strlen(a);
    *a = p[b];
    ++p;
    cout << p << endl;
    cout << a << endl;
    return 0;
}
