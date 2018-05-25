/*
** @file    Stack.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-03 23:43:39
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

template <typename T, int num>
class Stack
{
public:
	Stack() //可以设置栈空间的大小
	: _top(-1)
	{
		cout << "Stack()" << endl;
	}
	~Stack()
	{
		cout << "~Stack()" << endl;
	}
	bool isEmpty() const
	{
		return (_top == -1);
	}
	bool isFull() const
	{
		return (_top == (num- 1));
	}
	T &getTop()
	{
		return _pType[_top];
	}
	bool push(const T &val)
	{
		if(isFull())
			return false;
		_pType[++_top] = val;
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
	T _pType[num];
};

int main(void)
{
	Stack<int, 10> stack;
	cout << stack.isEmpty() << endl;
	stack.push(0);
	cout << stack.getTop() << endl;
	return 0;
}
