/*
** @file    my_class.cpp
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-20 04:51:41
*/
 
#include "my_class.h"
#include <iostream>
 
static void
pringMsg(int i)
{
    std::cout << i << std::endl;
}


void
MyClass::printMsg(void)
{
    std::cout << "I'm MyClass!" << std::endl;
}
