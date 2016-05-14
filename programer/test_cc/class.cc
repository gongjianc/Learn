/*
** @file    class.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-04-21 14:24:33
*/
 
#include <iostream>
#include <string>
 
using std::cout;
using std::endl;

class NoDefault{
public:
    NoDefault(std::string str)
    : _str(str)
    {}
private:
    std::string _str;
};

class A{
public:
    A()
    { cout << "A" << endl; }
    A(const A &rhs)
    { cout << "A(const A &)" << endl; }
    ~A()
    { cout << "~A" << endl; }
};

class B : public A{
public:
    B(A &a)
    : _a(a)
    { cout << "B" << endl; }
    ~B()
    { cout << "~B" << endl; }
private:
    A _a;
};

void test()
{
    class B{
    public:
        B(void)
        { cout << "B\t"; }
        ~B(void)
        { cout << "~B\t"; }
    };
    struct C{
        C(void)
        { cout << "C\t"; }
        ~C(void)
        { cout << "~C\t"; }
    };
    struct D : B{
        D()
        { cout << "D\t"; }
        ~D()
        { cout << "~D\t"; }
    private:
        C c;
    };
    D d;
}

int main(void)
{
    cout << "--------------" << endl;
    test();
    cout << endl;
    return 0;
}
