/*
** @file    MyString.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-03 05:53:15
*/
 
#include <iostream>
#include <string.h>
using std::endl;
using std::cout;
using std::ostream;

class String
{
	friend	ostream &operator<<(ostream &os, const String &rhs); 
private:
	//char的代理类
	class CharProxy
	{
	public:
		CharProxy(String &theString, size_t idx)
		: _theString(theString)
		, _idx(idx)
		{	
			cout << "CharProxy(String &, size_t)" << endl;	
		}
		CharProxy &operator=(const char &ch)
		{
			if(_theString.useCount() > 1){
				--_theString._pstr[_theString.size() + 1];
				char *pTemp = new char[_theString.size() + 2];
				strcpy(pTemp, _theString._pstr);
				_theString._pstr = pTemp;
				_theString._pstr[_theString.size() + 1] = 1;
			}
			_theString._pstr[_idx] = ch;
			return *this;
		}

		operator char() const
		{
			return _theString._pstr[_idx];
		}
	private:
		String &_theString;
		size_t _idx;
	};
public:
	String();
	String(const char *pstr);
	String(const String &rhs);
	String &operator=(const String &rhs);
	~String();

	size_t size() const
	{
		return strlen(_pstr);
	}
	size_t useCount() const
	{
		return _pstr[size() + 1];
	}
	CharProxy operator[](size_t idx);

private:
	char *_pstr;
};

String::String()
{
	cout << "String::String()" << endl;
	_pstr = new char[2];//开辟空间 2 个char的空间
	_pstr[1] = 1;//对引用计数初始化
}

String::String(const char *pstr)
{
	cout << "String::String(const char *)" << endl;
	size_t len = strlen(pstr);
	_pstr = new char[len + 2];
	_pstr[len + 1] = 1;
	strcpy(_pstr, pstr); //字符串的复制
}

String::String(const String &rhs)
: _pstr(rhs._pstr)
{
	cout << "String::String(const String &)" << endl;
	++_pstr[size()+ 1];
}

String &String::operator=(const String &rhs)
{
	cout << "String::operator=(const String &)" << endl;
	if(this != &rhs){
		if(0 == (--_pstr[size() + 1]))
			delete []_pstr;
		//strcpy(_pstr, rhs._pstr);
		_pstr = rhs._pstr; //指针的复制
		++_pstr[size() + 1];
	}
	return *this;
}


String::~String()
{
	if((--_pstr[size() + 1]) == 0){
		delete []_pstr;
		cout << "String::~String()" << endl;
	}
}

ostream &operator<<(ostream &os, const String &rhs)
{
	os << rhs._pstr;
	return os;
}

String::CharProxy String::operator[](size_t idx)
{
	return String::CharProxy(*this, idx);
}

int main(void)
{
	String str1;
	cout << str1 << endl;
//	String str2("hello,world");
	String str2 = "Hello,world";
	cout << "str2: " << str2 << endl;
	String str3 = str2;
	cout << str3 << endl;
	cout << str3.useCount() << endl;
	String str4 = "wangdao";
	cout << str4 << endl;
	str4 = str2;
	cout << "After str4 = str2, str4: " << str4 << endl;
	cout << str4.useCount() << endl;
	str4[0] = 'h';
	cout << "str4: " << str4 << endl;
	cout << str4.useCount() << endl;
	cout << "str2: " << str2 << endl;
	cout << str2.useCount() << endl;
	return 0;
}

