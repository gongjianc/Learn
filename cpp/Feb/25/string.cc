 ///
 /// @file    string.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-26 05:22:03
 ///
 
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

int main(void)
{
	string s1 = "hello";
	string s2 = "world";
	
	string s3 = s1 + s2;
	cout << "s3 is " << s3 << endl;

	s3.push_back('X');
	cout << "s3 now is " << s3 << endl;

	s1.append(s2);
	cout << "s1 is " << s1 << endl;

	string s4 = s3.substr(s3.find("wor",3),3);
	cout << "s4 is " << s4 << endl;

	for(int idx = 0; idx != s3.size(); idx++){
		cout << s3[idx] << endl;
		//对
	}

	cout << "iterator style" << endl;
	string::iterator it;
	for(it = s3.begin(); it != s3.end(); ++it){
		cout << *it << endl;
	}

	return 0;
}
