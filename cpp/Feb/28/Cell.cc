 ///
 /// @file    Cell.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-27 22:44:23
 ///
 
#include <iostream>

using std::cout;
using std::endl;

class Cell
{
public:
	Cell()
	: f(b)
	, g(d)
	{}

	void print()
	{
		cout << "hello" << endl;
	}

private:
	int a;
	float c;
	short e[5];
	char b;

	double d;

	char &f;
	double &g;
	static int h;
};
	
int Cell::h	= 0;

int main(void)
{
	float m;
	cout << "sizeof(float)" << sizeof(m) << endl;
	Cell cell;
	cout << "sizeof(cell)" << sizeof(cell) << endl;
	

	return 0;
}
