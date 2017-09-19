/*
** @file    hasptr.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-14 04:44:29
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

class HasPtr{
public:
    HasPtr(const std::string &s = std::string())
    : _pStr(new std::string(s))
    , i(0)
    {}
    ~HasPtr()
    {
        delete _pStr;
    }

private:
    std::string *_pStr;
    int i;
};
