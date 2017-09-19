/*
** @file    using_declaration.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-16 00:10:58
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

class Base{
public:
    std::size_t size() const
    { 
        cout << "size()" << endl;
        return _n; 
    }
protected:
    std::size_t _n;
};

class Derived : private Base{
public:
    using Base::size;
protected:
    using Base::_n;
};

int main(void)
{
    Derived d;
    d.size();

    return 0;
}
