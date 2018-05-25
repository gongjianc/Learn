/*
** @file    Stack.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-03 23:43:39
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

class Stack
{
public:
	Stack(int size = 10) //可以设置栈空间的大小
	: _top(-1)
	, _size(size)
	, _pInt(new int[_size])
	{
		cout << "Stack()" << endl;
	}
	~Stack()
	{
		delete []_pInt;
		cout << "~Stack()" << endl;
	}
	bool isEmpty() const
	{
		return (_top == -1);
	}
	bool isFull() const
	{
		return (_top == (_size - 1));
	}
	int &getTop()
	{
		return _pInt[_top];
	}
	bool push(const int &val)
	{
		if(isFull())
			return false;
		_pInt[++_top] = val;
		return true;
	}
	bool pop()
	{
		if(isEmpty())
			return false;
		--_top;
		return true;
	}
private:
	int _top;
	int _size;
	int *_pInt;
};

int main(void)
{
	Stack stack;
	cout << stack.isEmpty() << endl;
	stack.push(0);
	cout << stack.getTop() << endl;
	return 0;
}
