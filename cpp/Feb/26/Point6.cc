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

	Point & operator =(const Point & rhs); //default parameter this in the first place 
										   //& modify 
	void print();

private:
	int _ix;
	int _iy;
};

//Point(int ix = 0, int iy = 0); //没有必要
Point::Point(int ix, int iy)
: _ix(ix)
, _iy(iy)
{
	cout << "Point::Point(int, int)" << endl;
}

Point & Point::operator=(const Point & rhs)
{
	cout << "Point & operator =(const Point &)" << endl;
	_ix = rhs._ix;
	_iy = rhs._iy;

}
void Point::print()
{
	cout << "(" << _ix
		 << "," << _iy
		 << ")" << endl;
}

int main(void)
{
	Point pt1(3, 4);
	cout << "pt1 :";
	pt1.print();

	Point pt2;
	cout << "pt2 :";
	pt2.print();

	pt2 = pt1;
	cout << "After = pt2 :";
	pt2.print();
	return 0;
}
