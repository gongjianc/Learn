/*
** @file    class2.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-04-21 16:53:58
*/
 
#include <iostream>
#include <string>
 
using std::cout;
using std::endl;

class A{
public:
    A()
    : _a(0)
    {}
    virtual void fun() {}
private:
    int _a;
};

class B : public A{
public:
    B() : _b(1) {}
private:
    int _b;
};

class C : public A{
public:
    C() : _c(2) {}
private:
    int _c;
};

class D : public B, public C{
public:
    D()
    : _d(1)
    {}
private:
    int _d;
};

int main(void)
{
    D d;
    cout << "sizeof(d) = " << sizeof(d) << endl;
    return 0;
}

