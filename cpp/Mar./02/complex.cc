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

	double getReal() const
	{
		return _real;
	}

	double getImag() const
	{
		return _imag;
	}
	
	void display()
	{
		cout << _real << "+" << _imag << "i" << endl;
	}


private:
	double _real;
	double _imag;
};

Complex operator +(const Complex &lhs, const Complex &rhs)
{
	return Complex(lhs.getReal() + rhs.getReal() , lhs.getImag() + rhs.getImag() );
}

int main(void)
{
	Complex cx1(1.0, 2.0);
	Complex cx2(3.0, 4.0);
	cx1.display();
	cx2.display();

	Complex cxRes = cx1 + cx2;
	cxRes.display();

	return 0;
}
