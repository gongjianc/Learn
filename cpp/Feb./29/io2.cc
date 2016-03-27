 ///
 /// @file    io2.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-28 17:41:59
 ///
 
#include <iostream>
#include <string>
#include <limits>

using std::cout;
using std::cin;
using std::endl;

int main(void)
{
	int ival;

	while(cin >> ival, !cin.eof()){
		if(cin.bad())
			cout << "IO stream corrupted!" << endl;

		if(cin.fail()){
			cout << "DATA illegal, try again!" << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		cout << "ival = " << ival << endl;

	}
	return 0;
}
	

