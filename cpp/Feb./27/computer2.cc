 ///
 /// @file    computer.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-27 06:17:30
 ///
 
#include <iostream>
#include <string.h>

using std::cout;
using std::endl;

class Computer
{
public:
	Computer(const char* name, const float price);
	~Computer();

	static void print_total();
	static void print(Computer &com);

private:
	char *_pName;
	float _fPrcie;
	static float _totalPrice; //static数据成员被整个类共享
							  //他的初始化要在类声明之外进行
							  //不占用对象的存储空间
};

Computer::Computer(const char* name, const float price)
: _fPrcie(price)
{
	int len = strlen(name);
	_pName = new char [len + 1];
	memcpy(_pName, name, len); //memcpy更安全
	_totalPrice += _fPrcie;
}

Computer::~Computer()
{
	delete []_pName;
	_totalPrice -= _fPrcie;
}

void Computer::print_total()
{
	cout << "Total Price:" << _totalPrice << endl; //初始化不需要static
}

void Computer::print(Computer &com)
{
	cout << "Name:" << com._pName << endl;
	cout << "Price:" << com._fPrcie << endl;
}

float Computer::_totalPrice = 0;

int main(void)
{
	Computer pc1("IBM", 7000);
	cout << "After 1st computer bought," << endl;
	pc1.print(pc1);

	Computer::print(pc1); //static成员函数可以直接通过类名调用
	pc1.print_total();
	Computer::print_total(); //对于静态成员函数 可以不定义对象
	return 0;
}

