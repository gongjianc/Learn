 //
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
	~Student();

	void setId(int id);
	void setName(const char *name);
	void print();

private:
	int _id;
	char *_pName;

	void *operator new(size_t size);
	void operator delete(void *p);
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

void Student::setId(int id)
{
	_id = id;
}

void Student::setName(const char *name)
{
	int len = strlen(name);
	_pName = new char[len + 1];
	strcpy(_pName, name);
}

void *Student::operator new(size_t size)
{
	cout << "operator new(size_t)" << endl;
	void *p = malloc(size);
	return p;
}

void Student::operator delete(void *p)
{
	cout << "operator delete(void*)" << endl;
	free(p);
}

void Student::print()
{
	cout << "Id: " << _id << endl;
	cout << "Name " << _pName << endl;
}

int main(void)
{

	Student st1;
	st1.setId(89);
	st1.setName("John");
	st1.print();

	return 0;
}
