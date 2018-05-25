 ///
 /// @file    nullpointer.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-27 23:56:14
 ///
 
#include <iostream>

using std::cout;
using std::endl;

class NullPointerCall
{
public:
	static void test1();
	void test2();
	void test3(int iTest);
	void test4();

private:
	static int _iStatic;
	int _iTest;
};

int NullPointerCall::_iStatic = 0;

void NullPointerCall::test1()
{
	cout << "_iStatic=" << _iStatic << endl;
}

void NullPointerCall::test2()
{
	cout << "very good" << endl;
}

void NullPointerCall::test3(int iTest)
{
	cout << "iTest= " << iTest << endl;
}

void NullPointerCall::test4()
{
	cout << "_iTest = " << _iTest << endl;
}

int main(void)
{
	NullPointerCall * pNullPointer = NULL;

	pNullPointer->test1();
	pNullPointer->test2();
	pNullPointer->test3(3);
	//pNullPointer->test4(); //this

	return 0;
}

