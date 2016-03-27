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
class SingleImpl
{
private:
	class Singleton
	{
	public:
		static Singleton *getInstance()
		{
			if(_pInstance == NULL)
				_pInstance = new Singleton();
			return _pInstance;
		}
#if 0
		static void destroy()
		{
			cout << "Singleton::destroy()" << endl;
			delete _pInstance; //when use new(operator new()) to creat a object, when delete it, step is : call ~XXX()  call operator delete(); 
		}
#endif
		~Singleton()
		{
			cout << "Singleton::~Singleton()" << endl;
		}
	private:
		Singleton(); //这个函数私有化
		static Singleton *_pInstance;
	};

public:
	SingleImpl()
	: _pSingleImpl(Singleton::getInstance())
	{
		cout << "SingleImpl()" << endl;
	}
	~SingleImpl()
	{
		delete _pSingleImpl;
		cout << "~SingleImpl()" << endl;
	}

private:
	Singleton *_pSingleImpl;
};

SingleImpl::Singleton::Singleton()
{
	cout << "Singleton::Singleton()" << endl;
}

SingleImpl::Singleton *SingleImpl::Singleton::_pInstance = NULL; //why 

int main(void)
{
	SingleImpl();
	return 0;
}

