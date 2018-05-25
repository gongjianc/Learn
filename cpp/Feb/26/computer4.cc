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
	Computer & operator =(const Computer & rhs);

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
	_pbrand = new char[len + 1];
	strcpy(_pbrand,rhs._pbrand);
}

Computer::~Computer()
{
	cout << "Computer::~Computer()" << endl;
	delete []_pbrand;
}

Computer & Computer::operator =(const Computer & rhs)
{
	cout << "Computer::operator =(const Computer &)" << endl;
	if(this == &rhs)
		return *this; //自复制
					  //this指针代表当前对象的地址
	delete []_pbrand; //先释放当前指针的堆空间
					  //为什么没有这一步也可以？
	_fprice = rhs._fprice;
	int len = strlen(rhs._pbrand);
	_pbrand = new char[len + 1];
	strcpy(_pbrand,rhs._pbrand);

}
void Computer::print()
{
	cout << "Brand:" << _pbrand << endl;
	cout << "Price:" << _fprice << endl;
}

int main()
{

	Computer pc1("dell",7000);
	pc1.print();
	
	Computer pc2("mac",10000);
	pc2.print();
	cout << endl << endl;
	pc2 = pc1;
	pc2.print();

	cout << endl;
	Computer pc3 = pc1;
	pc3.print();

	
	return 0;
}
