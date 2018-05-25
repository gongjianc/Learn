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
	Point(const Point & rhs);

	void print();

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

int main(void)
{

	return 0;
}

