/*
** @file    virtual_functions.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-16 01:44:55
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

class Base{
public:
    virtual int fcn()
    //int fcn()
    {
        cout << "Base::fcn()" << endl;
        return 0;
    }
};

class D1 : public Base{
public:
    int fcn(int i)
    {
        cout << "D1::fcn(int)" << endl;
        return i;
    }
    virtual void f2()
    {
        cout << "D1::f2()" << endl;
    }
};

class D2 : public D1{
public:
    int fcn(int)
    {
        cout << "D2::fcn(int)" << endl;
        return 1;
    }
    int fcn()
    {
        cout << "D2::fcn()" << endl;
        return 2;
    }
    void f2()
    {
        cout << "D2::f2()" << endl;
    }
};

int main(void)
{
    Base bobj;
    D1 d1obj;
    D2 d2obj;
    Base *bp1 = &bobj, *bp2 = &d1obj, *bp3 = &d2obj;
    bp1->fcn();
    bp2->fcn();
    bp3->fcn();
    D1 *d1p = &d1obj, *d2p = &d2obj;
    //bp2->f2();
    d1p->f2();
    d2p->f2();
    return 0;
}
