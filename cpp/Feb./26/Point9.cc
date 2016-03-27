 ///
 /// @file    Point.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-26 18:09:38
 ///
 
#include <iostream>

using std::cout;
using std::endl;

class Point
{
public:
	//考虑类完成那些功能
	Point(int ix = 0, int iy = 0); //当函数的声明与定义分开的时候,是需要在定义的时候提供缺省的参数
	void print();

	Point(const Point & rhs);

private:
	const int _ix; //只能在初始化列表中进行
	const int _iy;
};

//Point(int ix = 0, int iy = 0); //没有必要
Point::Point(int ix, int iy)
: _ix(ix)
, _iy(iy)
{
	cout << "Point::Point(int, int)" << endl;
}

Point::Point(const Point & rhs)
: _ix(rhs._ix)
, _iy(rhs._iy)
{
	cout << "Point::Point(const Point &)" << endl;
}

void Point::print()
{
	cout << "(" << _ix
		 << "," << _iy
		 << ")" << endl;
}

class Line
{
public:
	Line(int x1, int y1, int x2, int y2);
	Line(const Line & rhs);

	void draw();
private:
	Point _pt1;
	Point _pt2;
};

Line::Line(int x1, int y1, int x2, int y2)
: _pt1(x1, y1)
, _pt2(x2, y2)
{
	cout << "Line::Line(int,int,int,int)" << endl;
}

Line::Line(const Line & rhs)
: _pt1(rhs._pt1)
, _pt2(rhs._pt2)
{
	cout << "Line::Line(const Line & rhs)" << endl;
}

void Line::draw()
{
	_pt1.print();
	cout << " to ";
	_pt2.print();
}

int main(void)
{
	Line l1(1, 2, 3, 4);
	l1.draw();

	Line l2 = l1;
	l2.draw();

	return 0;
}

