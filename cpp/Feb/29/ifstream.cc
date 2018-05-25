 ///
 /// @file    ifstream.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-28 19:01:03
 ///
 
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using std::ifstream;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ofstream;

int main(void)
{
	ifstream ifs("vector.cc");
	if(!ifs.good()){
		cout << "ifstream open error" << endl;
		return -1;
	}

	ofstream ofs("test.txt");
	if(!ofs.good()){
		cout << "ofstream open error" << endl;
	}

	string str;
	while(getline(ifs, str)){
			ofs << str << endl;
	}

	ifs.close();
	ofs.close();
	return 0;

			
}

int test(void)
{
	ifstream ifs("vector.cc");

	if(!ifs.good()){
		cout << "ifstream open error" << endl;
		return -1;
	}

	string str;
//	while(ifs >> str)
	vector<string> vecStr; //创建vector之后最好开辟一部分空间 指数幂增长 
	vecStr.reserve(100);

	while(getline(ifs, str)){
		cout << str << endl;
		vecStr.push_back(str);
	}

	vector<string>::iterator iter;
	for(iter = vecStr.begin(); iter != vecStr.end(); iter++)
		cout << *iter << endl;

	ifs.close(); //close the stream after finish using it
	return 0;
}
	
