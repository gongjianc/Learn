/*
** @file    template2.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-03 22:56:53
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

template <typename T>
T Max(T x, T y);

int main(void)
{
	int ix = 1, iy = 2;
	double dx = 3.9, dy = 2.9;
	cout << "Max(ix, iy) = " << Max(ix, iy) << endl;//隐式实例化
	cout << "Max(dx, dy) = " << Max<double>(dx, dy) << endl;//显试实例化
	return 0;
}

template <typename T>
T Max(T x, T y)
{
	return x > y ? x : y;
}	
