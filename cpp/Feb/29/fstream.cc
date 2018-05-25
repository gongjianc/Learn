/*
** @file    fstream.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-02-28 23:00:20
*/
 
#include <iostream>
#include <fstream>

using std::cin;
using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::fstream;

int test(void)
{
	ofstream ofs("test.txt", std::ios::app);
	if(!ofs.good()){
		cout << "ofstream open error!" << endl;
		return -1;
	}

	cout << ":" << ofs.tellp() << endl;
	ofs << "new line " << endl;
	cout << ":" << ofs.tellp() << endl;
	
	ofs.close();
	return 0;
}

int test1(void)
{
	test();
	//ate 对输入流起作用 定位到文件末尾
	ifstream ifs("test.txt", std::ios::ate);
	if(!ifs.good()){
		cout << "ifstream open error " << endl;
		return -1;
	}

	cout << ":" << ifs.tellg() << endl;
	
	ifs.close();
	return 0;
}

int main(void)
{
	//默认情况是读写模式 不会清空原有数据 std::ios::in | std::ios::out
	//fstream fs("test.txt");
	//app 文件默认的偏移位置是0，是一个相对的偏移位置
	//fstream fs("test1.txt", std::ios::in | std::ios::out | std::ios::app);
	fstream fs("test1.txt", std::ios::in | std::ios::out | std::ios::ate);
	
	if(!fs.good()){
		cout << "fstream open error" << endl;
		return -1;
	}

	int ival;
	for(int idx = 0; idx != 10; ++idx){
		cin >> ival;
		fs << ival << " ";
	}

	cout << ": " << fs.tellp() << endl;

//	for(int idx = 0; idx != 10; ++idx){


	fs.close();
	return 0;
}
