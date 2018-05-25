/*
** @file    virtual.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-17 19:23:53
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

class Base0{
public:
   Base0()
   {
       cout << "Base0::Base0()" << endl;
   }
   virtual void func1()
   { cout << "Base0::func1()" << endl; }
   virtual void func2()
   { cout << "Base0::func2()" << endl; }
   ~Base0()
   { cout << "Base0::~Base0()" << endl; }
};

class Base1 : public Base0{
public:
    Base1()
    {
        cout << "Base1::Base1()" << endl;
        func1();
    }
    virtual void func2()
    {
        cout << "Base1::func2()" << endl;
    }
    ~Base1()
    {
        cout << "Base1()::~Base1()" << endl;
        func2();
    }
};

class SubClass : public Base1{
public:
    virtual void func1()
    { cout << "SubClass::func1()" << endl; }
    virtual void func2()
    { cout << "SubClass::func2()" << endl; }
    ~SubClass()
    { cout << "SubClass::~SubClass()" << endl; }
};

int main(void)
{
    SubClass sc;
    SubClass *pSc = &sc;
    pSc->func1();
    pSc->func2();

    return 0;
}
