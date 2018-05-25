/*
** @file    stringstream.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-02-29 00:41:09
*/
 
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

int main(void)
{
	int ival = 512;
	int ival2 = 1024;

	stringstream ss; //会自动进行内存回收

	ss << "ival = " << ival << " " 
		 << "ival2 = " << ival2
		 << endl;

	string str = ss.str(); //转换为一个字符串
	cout << "str: " << str;

	string str2;
	while(ss >> str2)
		cout << str2 << endl;

	return 0;
}
	
