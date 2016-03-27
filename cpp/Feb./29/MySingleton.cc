/*
** @file    MySingleton.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-02-28 22:37:53
*/
 
#include <iostream>
#include 

using std::cout;
using std::endl;

class MySingleton
{
public:
	static MySingleton *getInstance()
	{
		if(_pInstance != NULL)
			return _pInstance;
		_pInstance = new MySingleton();
		return _pInstance;
	}

	static void destory()
	{
		delete _pInstance;
	}

	void print();
	~MySingleton()
	{
		cout << "MySingleton::~MySingleton()" << endl;
	}

private:
	MySingleton();
	static MySingleton *_pInstance;
};

MySingleton::MySingleton()
{
	cout << "MySingleton::MySingleton()" << endl;
}

void MySingleton::print()
{
	cout << "void print()" << endl;
}

MySingleton *MySingleton::_pInstance = NULL;

int main(void)
{
	MySingleton *mySingle = MySingleton::getInstance();
	mySingle->print();

	MySingleton::destory();

	return 0;
}
