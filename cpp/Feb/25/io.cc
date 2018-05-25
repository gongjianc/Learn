 ///
 /// @file    io.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-26 06:01:04
 ///
 
#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;

int main()
{
	string str;
	while(getline(cin, str)) //getline get a line 
		cout << str <<endl;

	return 0;
}
