/*
** @file    function.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-21 00:38:23
*/
 
#include <iostream>
#include <functional>
 
using std::cout;
using std::endl;

struct Foo{
    Foo(int num) : _num(num) {}
    void print_add(int i) const
    {
        cout << "Foo::print_add(int i) = " << _num + i << endl;
    }
    int _num;
};

void print_num(int i)
{
    cout << "print_num(int i) = " << i << endl;
}

struct PinrtNum{
    void operator()(int i) const
    {
        cout << "PinrtNum() = " << i << endl;
    }
};

int main(void)
{
    std::function<void(int)> f_display = print_num;
    f_display(-9);
    
    std::function<void(const Foo &, int)> f_add_display = &Foo::print_add;
    const Foo foo(314159);
    f_add_display(foo, 1);

    std::function<void(int)> f_display_obj = PinrtNum();
    f_display_obj(18);

    cout << "<---------------with bind-------------->" << endl;
    std::function<void(void)> f_display_31337 = std::bind(&print_num, 311337);
    f_display_31337();

    std::function<void(int)> f_add_display2 = std::bind(&Foo::print_add, foo, std::placeholders::_1);
    f_add_display2(2);

    std::function<void(int)> f_add_display3 = std::bind(&Foo::print_add, &foo, std::placeholders::_1);
    f_add_display3(3);

    return 0;
}
