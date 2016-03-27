/*
** @file    complex.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-02 08:00:50
*/
 
#include <iostream>

using std::cout;
using std::endl;

class Complex
{
public:
	Complex(double real = 0, double imag = 0)
	: _real(real)
	, _imag(imag)
	{
		cout << "Complex(double, double)" << endl;
	}

//	Complex operator+(const Complex &lhs, const Complex &rhs) ? wrong
//	Complex operator+(const Complex &rhs)  ? right
//	成员函数默认有this参数 所以这种方式不直观
	Complex &operator+=(const Complex &rhs);
	Complex operator++(int); //后置加加 返回值的区别
	Complex &operator++();

	void display()
	{
		if(_imag > 0)
			cout << _real << "+" << _imag << "i" << endl;
		else if(_imag == 0)
			cout << _real << endl;
		else
			cout << _real << "-" << (-_imag) << "i" << endl;
	}
private:
	double _real;
	double _imag;
};

Complex &Complex::operator+=(const Complex &rhs)
{
	this->_real += rhs._real;
	this->_imag += rhs._imag;
	return *this;
}

//一般对于 + - * / 利用 += -= *= /= 重载
Complex operator+(const Complex &lhs, const Complex &rhs)
{
	//lhs += rhs; //? lhs is modified by const, so change the value of lsh is illegal
	Complex temp(lhs);
	temp += rhs;
	return temp;
}

Complex &Complex::operator++()
{
	++_imag;
	++_real;
	return *this;
}

Complex Complex::operator++(int)
{
	Complex temp(_real, _imag);
	++_real;
	++_imag;
	return temp; //实际已加 但返回临时保存的temp 
}

int main(void)
{
	Complex cx1(1.0, 2.0);
	Complex cx2(3.0, -4.0);
	cx1.display();
	cx2.display();
	cx1 += cx2;
	cx1.display();
	//Complex cxRes = cx1 + cx2;
	//cxRes.display();
	return 0;
}
