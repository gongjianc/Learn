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
	Point();
	Point(int ix, int iy); //当函数的声明与定义分开的时候,是需要在定义的时候提供缺省的参数

	~Point();

	void print();

private:
	int _ix;
	int _iy;
};

Point::Point()
: _ix(0)
, _iy(0)
{
	cout << "Point::Point()" << endl;
}

//Point(int ix = 0, int iy = 0); //没有必要
Point::Point(int ix, int iy)
: _ix(ix)
, _iy(iy)
{
	cout << "Point::Point(int, int)" << endl;
}

Point::~Point()
{
	cout << "Point::~Point()" << endl;
}

void Point::print()
{
	cout << "(" << _ix
		 << "," << _iy
		 << ")" << endl;
}

int main(void)
{
	Point arr[2] = {Point(1, 2), Point(3, 4)}; //产生匿名对象
	arr[0].print();
	
	Point *ppt = new Point(5, 6);
	ppt->print();
	delete ppt;
	ppt = NULL;  //防御性 操作 放置成为野指针

	return 0;
}
