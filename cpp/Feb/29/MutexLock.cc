/*
** @file    MutexLock.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-02-29 06:51:32
*/
 
#include <iostream>

using std::cout;
using std::endl;

class MutexLock
{
public:
	static void lock();
	static void unlock();
	~MutexLock()
	{
		cout << "~MutexLock()" << endl;
	}
private:
	static MutexLock *_pLock; 
	MutexLock()
	{
		cout << "MutexLock()" << endl;
	}
};

void MutexLock::lock()
{
	if(_pLock != NULL){
		cout << "resource using" << endl;
		//while(1);
		return;
	}

	_pLock = new MutexLock();
}

void MutexLock::unlock()
{
	delete _pLock;
	cout << "resource unlock" << endl;
}

MutexLock *MutexLock::_pLock = NULL;

int main(void)
{

	MutexLock::lock();
	MutexLock::lock();
	MutexLock::unlock();

	return 0;
}

