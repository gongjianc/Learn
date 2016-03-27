/*
** @file    left_right_value.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-10 18:10:53
*/
 
#include <iostream>
#include <vector>
 
using std::cout;
using std::endl;
using std::vector;

int main(void)
{
    //int a = 10;
    //int b = 120;
//    int pA = &b;
//    &(a + b);
    //&(++a);//左值
    //左值 有名字 能够再次使用
    //右值 没有名字 只能使用一次
    //字面值
    int &&m = 1;//左值引用 
    const int &n = 1;
    return 0;
}

