 ///
 /// @file    String.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-28 01:33:12
 ///
 
#include <iostream>
#include <string.h>

using std::cout;
using std::endl;

class String
{
public:
	String();
	String(const char *pstr);
	String(const String &rhs);
	String & operator=(const String &rhs);
	~String();

	void print();

private:
	char *_pStr;
};

String::String()
{
	cout << "String::String()" << endl;
	_pStr = new char[1];
}

String::String(const char* pstr)
{
	cout << "String::String(const char*) " << endl;
	int len = strlen(pstr);
	_pStr = new char[len + 1];
	strcpy(_pStr, pstr);
}

//默认的复制构造函数只是复制地址 析构谁出错
String::String(const String & rhs)
{
	cout << "String::String(const String &)" << endl;
	int len = strlen(rhs._pStr);
	_pStr = new char[len + 1];
	strcpy(_pStr, rhs._pStr);
}

String::~String()
{
	cout << "String::String()" << endl;
	delete []_pStr;
}

String & String::operator=(const String &rhs)
{
	cout << "String & String::operator=(const String &)" << endl;
	if(this == &rhs)
		return *this;

	delete []_pStr; //this 

	int len = strlen(rhs._pStr);
	_pStr = new char[len + 1];
	strcpy(_pStr, rhs._pStr);
	return *this;
}

void String::print()
{
	cout << "_pStr: " << _pStr << endl;
}

int main(void)
{
	String str1;
	str1.print();

	String str2 = "Hello,world";
	str2.print();
	String str3 = "wangdao";
	str3.print();

	str2 = str3;
	str2.print();
	
	String str4 = str3;
	str4.print();

	return 0;

}
