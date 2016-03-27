/*
** @file    oversee.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-11 23:35:47
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

class Base
{
public:
    //隐藏：在父子类之间，一旦函数名称相同（not virtual）名称相同
    //通过派生类的对象，只能看到派生类的函数
    int func(int i)
    {
        return _ix;
    }
protected:
    int _ix;
};

class Derived : public Base
{
public:
    int func()
    {
        _ix = 0;
        Base::_ix = 22;
        return _ix;
    }
public:
    int _ix;
};

int main(void)
{
    Derived obj;
    obj.func();
    //obj.func(3); //隐藏
    obj.Base::func(3);
    return 0;
}
