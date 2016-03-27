 ///
 /// @file    computer.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-27 06:17:30
 ///
 
#include <iostream>

using std::cout;
using std::endl;

class Computer
{
public:
	Computer(const float price);
	~Computer();

	void print();

private:
	float _fPrcie;
	static float _totalPrice; //static数据成员被整个类共享
							  //他的初始化要在类声明之外进行
							  //不占用对象的存储空间
};

Computer::Computer(const float price)
: _fPrcie(price)
{
	_totalPrice += _fPrcie;
}

Computer::~Computer()
{
	_totalPrice -= _fPrcie;
}

void Computer::print()
{
	cout << "Total Price:" << _totalPrice << endl; //初始化不需要static
}

float Computer::_totalPrice = 0;

int main(void)
{
	Computer pc1(7000);
	cout << "After 1st computer bought" << endl;
	pc1.print();

	Computer pc2(6000);
	cout << "After 2nd computer bought" << endl;
	pc2.print();

	Computer pc3(2500);
	cout << "After 3th computer bought" << endl;
	pc1.print();

	cout << "sizeof(pc1) " << sizeof(pc1) << endl;


	return 0;
}

