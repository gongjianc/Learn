/*
** @file    nested.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-03 00:37:49
*/
 
#include <iostream>

using std::cout;
using std::endl;

class Line
{
public:
	class Point
	{
	public:
	  	Point(int ix = 0, int iy = 0);
		void printPoint();
	private:
		int _ix;
		int _iy;
	};
public:
	Line(int x1, int y1, int x2, int y2);
	void printLine();
private:
	Point _p1;
	Point _p2;
};

Line::Line(int x1, int y1, int x2, int y2)
: _p1(x1, y1)
, _p2(x2, y2)
{
	cout << "Line(int, int, int, int)" << endl;
}

void Line::printLine()
{
	_p1.printPoint();
	cout << "---------->";
	_p2.printPoint();
	cout << endl;
}

Line::Point::Point(int ix, int iy)
: _ix(ix)
, _iy(iy)
{
	cout << "Line::Point(int, int)" << endl;
}

void Line::Point::printPoint()
{
	cout << "(" << _ix << "," << _iy << ")";
}

int main(void)
{
	Line line(1, 2, 3, 4);
	line.printLine();

	Line::Point pt(5, 6);
	pt.printPoint();
	cout << endl;
	return 0;
}
