/*
** @file    destructor_virtual.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-17 23:28:48
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

class Base{
public:
    Base()
    : _data(new char[64])
    {
        cout << "Base::Base()" << endl;
    }
    virtual
    ~Base()
    {
        cout << "Base::~Base()" << endl;
        delete []_data;
    }
private:
    char *_data;
};

class Derived : public Base{
public:
    Derived()
    : Base()
    , _ddata(new char[64])
    {
        cout << "Derived::Derived()" << endl;
    }
    ~Derived()
    {
        cout << "Derived::~Derived()" << endl;
        delete []_ddata;
    }
private:
    char *_ddata;
};

int main(void)
{
    Base *pBase = new Derived;
    //delete两个知识点
    //1. 虚函数覆盖
    //2. 派生类析构调用顺序
    delete pBase;
    return 0;
}
