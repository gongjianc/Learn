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
private:
	class AutoRelease
	{
	public:
		AutoRelease()
		{
			cout << "AutoRelease()" << endl;
		}

		~AutoRelease()
		{
			cout << "~AutoRelease()" << endl;
			if(_pInstance)
				delete _pInstance;
		}
	};
public:
	static Singleton *getInstance()
	{
		if(_pInstance == NULL)
			_pInstance = new Singleton();
		return _pInstance;
	}

	~Singleton()
	{
		cout << "Singleton::~Singleton()" << endl;
	}
private:
	Singleton(); //这个函数私有化
	static Singleton *_pInstance;

	static AutoRelease _autoRelease;
	//为什么用static修饰
	//对于全局定义的对象，在main函数之前就调用构造函数，
	//整个程序结束时调用析构函数
};

Singleton::Singleton()
{
	cout << "Singleton::Singleton()" << endl;
}

Singleton *Singleton::_pInstance = NULL; //why 
Singleton::AutoRelease Singleton::_autoRelease;

int main(void)
{
	Singleton * pSingle = Singleton::getInstance(); //静态成员函数可以直接用类名来调用
	Singleton * pSingle2 = Singleton::getInstance();	

	printf("pSingle = %x\n", pSingle);
	printf("pSingle2 = %x\n", pSingle2);

	return 0;
}

