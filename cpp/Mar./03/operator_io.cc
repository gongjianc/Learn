/*
** @file    complex.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-02 08:00:50
*/
 
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::ostream;
using std::istream;

class Complex
{
	friend ostream &operator<<(ostream &os, const Complex &rhs);
	friend istream &operator>>(istream &is, Complex &rhs);
public:
	Complex(double real = 0, double imag = 0)
	: _real(real)
	, _imag(imag)
	{
		cout << "Complex(double, double)" << endl;
	}
private:
	double _real;
	double _imag;
};

//<<的返回左值
ostream &operator<<(ostream &os, const Complex &rhs)
{
	if(rhs._imag > 0)
		os << rhs._real << "+" << rhs._imag << "i";
	else if(rhs._imag == 0)
		os << rhs._real;
	else 
		os << rhs._real << "-" << -rhs._imag << "i" ;
	return os;
}

istream &operator>>(istream &is, Complex &rhs)
{
	//?????????????????????????????
	is >> rhs._real;
	while(is.get() != '*');
	is >> rhs._imag;
	return is;
}

int main(void)
{
	Complex cx1(1.0, 2.0);
	Complex cx2(3.0, -4.0);
	cout << cx1 << endl;
	cout << cx2 << endl;
	Complex cx3;
	cin >> cx3;
	cout << cx3 << endl;
	return 0;
}
