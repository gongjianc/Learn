/*
** @file    test2.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-04-09 17:56:54
*/
 
#include <iostream>
#include <string.h>
 
using std::cout;
using std::endl;

void getMemory(char *p)
{
    p = (char *)malloc(11);
}

int main(void)
{
    char *str = "Hello";
    getMemory(str);
    strcpy(str, "Hello World");
    printf("%s\n", str);
    return 0;
}
