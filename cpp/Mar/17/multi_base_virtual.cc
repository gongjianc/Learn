/*
** @file    multi_base_virtual.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-17 23:37:23
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

class A{
public:
    virtual void a()
    {
        cout << "A::a()" << endl;
    }
    virtual void b()
    {
        cout << "A::b()" << endl;
    }
    virtual void c()
    {
        cout << "A::c()" << endl;
    }
};

class B{
public:
    virtual void a()
    {
        cout << "B::a()" << endl;
    }
    virtual void b()
    {
        cout << "B::b()" << endl;
    }
    void c()
    {
        cout << "B::c()" << endl;
    }
    void d()
    {
        cout << "B::d()" << endl;
    }
};

class C : public A, public B{
public:
    void a()
    {
        cout << "C::a()" << endl;
    }

    void c()
    {
        cout << "C::c()" << endl;
    }
    void d()
    {
        cout << "C::d()" << endl;
    }
};

int main(void)
{
    C c;
    //c.b();//二义性
    
    A *pA = &c;
    pA->a();
    pA->b();// A::b() 

    B *pB = &c;
    pB->a();
    pB->b();
    pB->c();//B::c()

    C *pC = &c;
    pC->a();
    //pC->b();//二义性
    cout << "sizeof(C) = " << sizeof(C) << endl;

    return 0;
}
