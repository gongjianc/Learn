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
	Computer(const Computer & rhs);

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

Computer::Computer(const Computer & rhs)
: _fprice(rhs._fprice)
{
	cout << "Computer::Computer(const Computer &)" << endl;
	int len = strlen(rhs._pbrand);
	_pbrand = new char[len + 1]; //系统是多少位的 
	strcpy(_pbrand,rhs._pbrand);
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


int main()
{
	
	Computer pc("Dell", 7000);
	pc.print();
	Computer pc1 = pc; //发生了什么？
	cout << "pc1 print" << endl;
	pc1.print();

	Computer pc2 = pc;
	cout << "pc2 print" << endl;
	pc2.print();


	return 0;
}
