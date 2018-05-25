/*
** @file    virtual1.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-15 06:00:24
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

class Base{
public:
    virtual int func(int x)
    {
        cout << "This is Base class." << endl;
        return x;
    }
};

class Derived : public Base{
public:
    //覆盖基类的虚函数时，必须要与基类的虚函数严格一致
    virtual int func(int x)
    {
        cout << "This is Derived Class." << endl;
        float y = x;
        return y;
    }
};

void test(Base &x)
{
    cout << "x = " << x.func(5) << endl;
}

int main(void)
{
    Base base;
    Derived derived;
    test(base);
    test(derived);

    return 0;
}
