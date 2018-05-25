 ///
 /// @file    computer.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-26 07:17:58
 ///
 
#include <iostream>
#include <string.h>

using std::cout;
using std::endl;

class Computer
{
public:
	Computer(const char *brand, float price);
	~Computer();

	void print();
private:
	char* _pbrand;
	float _fprice;
};

Computer::Computer(const char *brand, float price) //用const是对传入的参数的保护 防止指针被修改
: _fprice(price)
{
	cout << "Computer::Computer(const char *, float)" << endl;
	int len = strlen(brand);
	_pbrand = new char[len + 1]; //系统是多少位的 
	strcpy(_pbrand, brand);
}

Computer::~Computer()
{
	cout << "Computer::~Computer()" << endl;
	delete []_pbrand;
}

void Computer::print()
{
	cout << "Brand:" << _pbrand << endl;
	cout << "Price:" << _fprice << endl;
}

Computer com("mac", 10000);

int main()
{
	Computer pc("Dell", 7000);
	pc.print();
}
