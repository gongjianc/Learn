/*
** @file    friend_point.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-02 07:27:30
*/
 
#include <iostream>
#include <math.h>

using std::endl;
using std::cout;

class Point
{
	friend float distance(const Point &lhs, const Point &rhs);
public:
	Point(int ix = 0, int iy = 0)
	: _ix(ix)
	, _iy(iy)
	{
		cout << "Point(int, int )" << endl;
	}

	void display()
	{
		cout << "(" << _ix
			 << "," << _iy
			 << ")";
	}
private:
	int _ix;
	int _iy;
};

float distance(const Point &lhs, const Point &rhs)
{
	return sqrt((lhs._ix - rhs._ix) * (lhs._ix - rhs._ix)
					+ (lhs._iy - rhs._iy) * (lhs._iy - rhs._iy));
}

int main(void)
{
	Point pt1(1, 2),pt2(4, 5);
	pt1.display();
	cout << "and";
	pt2.display();

	cout << "distance: " << distance(pt1, pt2) << endl;

	return 0;
}
