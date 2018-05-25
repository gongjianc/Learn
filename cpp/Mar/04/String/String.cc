/*
** @file    String.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-03 04:26:51
*/
 
#include "String.hh"
#include <string.h>
//无参构造函数
String::String()
: _pStr(new char[1]) //为什么是坑
{
    std::cout << "String()" << std::endl;
}
String::String(const char *str)
{
    std::cout << "String(const char *)" << std::endl;
    int len = strlen(str);
    _pStr = new char[len + 1];
    strcpy(_pStr, str);
}
//复制构造函数
String::String(const String &rhs)
{
    std::cout << "String(const String &)" << std::endl;
    int len = strlen(rhs._pStr);
    _pStr = new char[len + 1];
    strcpy(_pStr, rhs._pStr);
}

String::~String()
{
    std::cout << "~String()" << std::endl;
    delete []_pStr;
}
//赋值运算符重载
String &String::operator=(const String &rhs)
{
    std::cout << "String &operator=(const String &)" << std::endl;
    delete []_pStr;//防止内存泄漏
    _pStr = new char[strlen(rhs._pStr) + 1];
    strcpy(_pStr, rhs._pStr);
    return *this;
}
//赋值 右值是字符串
String &String::operator=(const char *pstr)
{
    std::cout << "String &operator=(const char *)" << std::endl;
    delete []_pStr;
    _pStr = new char[strlen(pstr) + 1];
    strcpy(_pStr, pstr);
    return *this;
}

String &String::operator+=(const String &rhs)
{
    std::cout << "String &operator+=(const String &)" << std::endl;
    strcat(_pStr, rhs._pStr);
    return *this;
}
String &String::operator+=(const char *pstr)
{
    std::cout << "String &operator+=(const char *)" << std::endl;
    strcat(_pStr, pstr);
    return *this;
}

char &String::operator[](std::size_t index)
{
    std::cout << "String &operator[](std::size_t)" << std::endl;
    return _pStr[index];
}
const char &String::operator[](std::size_t index) const
{
    std::cout << "const String &operator[](std::size_t)" << std::endl;
    return _pStr[index];
}

bool operator==(const String &lhs, const String &rhs)
{
    std::cout << "bool operator==(cost String &, const String &)" << std::endl;
    if(!strcmp(lhs._pStr, rhs._pStr))
        return true;
    else
        return false;
}
bool operator!=(const String &lhs, const String &rhs)
{
    std::cout << "bool operator!=(cost String &, const String &)" << std::endl;
    if(!strcmp(lhs._pStr, rhs._pStr))
        return false;
    else
        return true;
}

std::size_t String::size() const
{
    return strlen(_pStr);
}

const char* String::c_str() const
{
    return _pStr;
}

bool operator<=(const String &, const String &);
bool operator>=(const String &, const String &);
bool operator>(const String &, const String &);
bool operator<(const String &, const String &);
    
std::ostream &operator<<(std::ostream &os, const String &s)
{
    os << s._pStr;
    return os;
}
std::istream &operator>>(std::istream &is, String &s)
{
    is >> s._pStr; 
    return is;
}

String operator+(const String &str1, const String &str2)
{
    String str(str1);
    str += str2;
    return str;
}
String operator+(const char *str1, const String &str2)
{
    String str(str1);
    str += str2;
    return str;
}

String operator+(const String &str1,const char *str2)
{
    String str(str1);
    str += str2;
    return str;
}
