/*
** @file    typecast.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-02 23:45:24
*/
 
#include <iostream>

using std::endl;
using std::cout;
using std::ostream;

class Point;
class AnotherPoint
{
	friend ostream &operator<<(ostream &os, const AnotherPoint &rhs);
	friend class Point;
public:
	AnotherPoint(double dx = 0, double dy = 0)
	: _dx(dx)
	, _dy(dy)
	{
		cout << "AnotherPoint(double, double)" <<endl;
	}

private:
	double _dx;
	double _dy;
};

class Point
{
	friend ostream &operator<<(ostream &os, const Point &rhs);
public:
	Point(int ix = 0, int iy = 0)
	: _ix(ix)
	, _iy(iy)
	{
		cout << "Point(int, int)" <<endl;
	}
	Point(AnotherPoint ap)
	: _ix(ap._dx)
	, _iy(ap._dy)
	{
		cout << "Point(AnotherPoint)" << endl;
	}

	
private:
	int _ix;
	int _iy;
};

ostream &operator<<(ostream &os, const AnotherPoint &rhs)
{
	os << "(" << rhs._dx << "," << rhs._dy << ")";
	return os;
}

ostream &operator<<(ostream &os, const Point &rhs)
{
	os << "(" << rhs._ix << "," << rhs._iy << ")";
	return os;
}

int main(void)
{
	Point pt1;
	cout << 5 << "transform to ";
	pt1 = 5;
	cout << pt1 << endl;
	AnotherPoint ap1(1.5);
	cout << ap1 << "transform to";
	pt1 = ap1;
	cout << pt1 << endl;
	return 0;
}
