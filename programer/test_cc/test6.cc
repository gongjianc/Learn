/*
** @file    test6.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-04-28 13:58:23
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

class Base{
public:
    virtual void f() 
    { cout << "Base::f" << endl; }
    virtual void g()
    { cout << "Base::g" << endl; }
    virtual void h()
    { cout << "Base::h" << endl; }
};

int main(void)
{
    typedef void (*Fun) (void);
    Base b;
    Fun pFun = NULL;
    cout << "虚函数地址：" << (long*)(&b) << endl;
    //64位系统指针是8字节，int是4个字节
    cout << "虚函数表 - 第一个函数的地址：" << (long*)*(long*)(&b) << endl;
    pFun = (Fun)*((long*)*(long*)(&b));
    pFun();
    pFun = (Fun)*((long*)*(long*)(&b) + 1);
    pFun();
    pFun = (Fun)*((long*)*(long*)(&b) + 2);
    pFun();
    return 0;
    }




