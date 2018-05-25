/*
** @file    destructor.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-11 23:22:08
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

class A
{
public:
    A(int xp = 0)
    : _ix(xp)
    {
        cout << "A(int)" << endl;
    }
    ~A()
    {
        cout << "~A()" << endl;
    }
private:
    int _ix;
};

class B
{
public:
    B()
    {
        cout << "B()" << endl;
    }
    ~B()
    {
        cout << "~B()" << endl;
    }

};

class C : public A
{
public:
    C(int xp, int yp)
    : A(xp)
    , _iy(yp)
    , _b()//可以不写，会自动调用B的构造函数
    {
        cout << "C(int, int)" << endl;
    }
    ~C()
    {
        cout << "~C()" << endl;
    }
private:
    int _iy;
    B _b;
};

int main(void)
{
    C c(1, 2);

    return 0;
}
