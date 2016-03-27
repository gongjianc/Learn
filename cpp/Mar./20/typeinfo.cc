/*
** @file    typeinfo.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-18 19:37:02
*/
 
#include <iostream>
#include <string.h>
#include <typeinfo>
 
using std::cout;
using std::endl;

class A1{
public:
    virtual void print()
    {
        cout << "A1::print()" << endl;
    }
};

class B1 : public A1{};

class A2{
public:
    void print()
    {
        cout << "A2::print()" << endl;
    }
};

class B2 : public A2{};

template <class T>
T Max(T a, T b)
{
    if(typeid(T) == typeid(char *)){
        char *pa = (char *)a;
        char *pb = (char *)b;
        return (strcmp(pa, pb)) > 0 ? a : b;
    }else
        return a > b ? a : b;
}

int main(void)
{
    cout << typeid(5).name() << endl;
    cout << typeid(double).name() << endl;
    
    A1 *pa1 = new B1;
    cout << typeid(*pa1).name() << endl;

    A2 *pa2 = new B2;//静态
    cout << typeid(*pa2).name() << endl;
    
    cout << Max<int>(22, 23) << endl;

    return 0;
}
