 ///
 /// @file    Studeng.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-28 06:28:10
 ///
 
#include <string.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

class Student
{
public:
	Student();

	void print();
	void destroy();
	
	void *operator new(size_t sz)
	{
		cout << "operator new(size_t)" << endl;
		void *p = malloc(sz);
		return p;
	}

	void operator delete(void *p)
	{
		cout << "operator delete(void*)" << endl;
		free(p);
	}

private:
	int _id;
	char *_pName;
	~Student();
};

Student::Student()
: _id(0)
, _pName(NULL)
{
	cout << "Student::Student()" << endl;
}

Student::~Student()
{
	cout << "Student::~Student()" << endl;
}

void Student::destroy()
{
	//this->~Student();
	cout << "Student::destroy()" << endl;
	delete this;
}

void Student::print()
{
	cout << "ID: " << _id << endl;
	cout << "Name: " << _pName << endl;
}

int main(void)
{
	Student *pst = new Student();
//	pst->print();
	pst->destroy();
//	delete pst; //为什么直接调用delete
	
	return 0;
}
