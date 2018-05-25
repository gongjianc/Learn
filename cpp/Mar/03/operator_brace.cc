/*
** @file    operator_brace.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-02 19:28:26
*/
 
#include <iostream>

class Demo
{
public:
	double operator()(double x, double y)
	{
		return x > y ? x : y;
	}

	double operator()(double x, double y, double z)
	{
		return (x + y) * z;
	}
};

int main(void)
{
	Demo demo;//function object
	std::cout << demo(2.5, 1.2) << std::endl;

	return 0;
}
