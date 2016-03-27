 ///
 /// @file    Singleton.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-28 05:58:07
 ///
 
#include <iostream>
#include <stdio.h>

using std::cout;
using std::endl;

//栈对象，每次调用一个函数就可以生成它的一个对象
//只有堆对象是可以跨函数的 
class Singleton
{
public:
	static Singleton *getInstance()
	{
		if(_pInstance == NULL)
			_pInstance = new Singleton();
		return _pInstance;
	}
	static void destroy()
	{
		cout << "Singleton::destroy()" << endl;
		delete _pInstance; //when use new(operator new()) to creat a object, when delete it, step is : call ~XXX()  call operator delete(); 
	}

	~Singleton()
	{
		cout << "Singleton::~Singleton()" << endl;
	}
private:
	Singleton(); //这个函数私有化
	static Singleton *_pInstance;
};

Singleton::Singleton()
{
	cout << "Singleton::Singleton()" << endl;
}

Singleton *Singleton::_pInstance = NULL; //why 

int main(void)
{
	Singleton * pSingle = Singleton::getInstance(); //静态成员函数可以直接用类名来调用
	Singleton * pSingle2 = Singleton::getInstance();	

	printf("pSingle = %x\n", pSingle);
	printf("pSingle2 = %x\n", pSingle2);

	Singleton::destroy();
	return 0;
}

