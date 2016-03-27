/*
** @file    template1.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-03 22:40:12
*/
 
#include <iostream>
 
using std::cout;
using std::endl;
using std::string;

template <typename T>
T add(const T &a, const T &b)
{
	return a + b;
}

template <typename T, int NUM>
T fun(T a)
{
	return a * NUM;
}

int main(void)
{
	cout << fun<int, 4>(3) << endl;
	cout << fun<double, 6>(4.0) << endl;
	cout << "10 + 20 = " << add(10, 20) << endl;
	cout << "1.0 + 2.0 = " << add(1.0, 2.0) << endl;

	string str1("hello"),str2("world");
	cout << "str1 + str2: " << add(str1, str2) << endl;
	return 0;
}

