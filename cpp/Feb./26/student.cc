 ///
 /// @file    student.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-26 08:04:03
 ///
 
#include <iostream>
#include "student.h"
#include <string.h>

using std::cout;
using std::endl;

void Student::setName(char *name)
{
	strcpy(_name,name);
}

void Student::setNum(int num)
{
	num_ = num;
}

void Student::print()
{
	cout << "name is " << _name << endl
		 << "Nume is " << num_ << endl;
}

int main(void)
{
	Student st;
	st.setNum(12345);
	st.setName("Li Ming");
	cout << "The student info is " << endl;
	st.print();
	return 0;
}
