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
	Point(const Point & rhs); //const控制函数对参数的行为 保护 
						      //引用 减少函数开销
							  //避免无限递归 , 栈溢出
	void print();

private:
	int _ix;
	int _iy;
};

Point::Point()
: _ix(0)
, _iy(0) //
{
	cout << "Point::Point()" << endl;
}
//Point(int ix = 0, int iy = 0); //没有必要
Point::Point(int ix, int iy) //通过初始化列表来完成数据成员的初始化效率更高
: _ix(ix)
, _iy(iy)
{
	cout << "Point::Point(int, int)" << endl;
}

Point::Point(const Point & rhs)
: _ix(rhs._ix)
, _iy(rhs._iy) //位于Point类里面进行的定义，所以可以访问私有成员 
{
	cout << "Point::Point(const Point &)" << endl;
}

void Point::print()
{
	cout << "(" << _ix
		 << "," << _iy
		 << ")" << endl;
}

void func(Point pt)
{
	cout << "func()" << endl;
	pt.print();
}

Point getPoint()
{
	cout << "getPoint()" << endl;
	Point pt(5,6);
	return pt;
}

int main(void)
{
	Point pt1(3, 4);
	pt1.print();

	Point pt2 = pt1;
	cout << "p2" << endl;
	pt2.print();

	cout << "combine" << endl;
	func(pt2);

	cout << endl 
		 << "return class"
		 << endl;

	getPoint(); //看不到返回值 编译器做了优化 需要gcc关闭RVO的选项 -fno-elide-constructors
	return 0;
}
