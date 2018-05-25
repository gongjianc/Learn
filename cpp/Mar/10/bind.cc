/*
** @file    bind.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-10 01:13:17
*/
 
#include <iostream>
#include <functional>
 
using std::cout;
using std::endl;
using namespace std::placeholders;//namespace?

void f(int n1, int n2, int n3, const int &n4, int n5)
{
    cout << "n1 = " << n1 << endl
         << "n2 = " << n2 << endl
         << "n3 = " << n3 << endl
         << "n4 = " << n4 << endl
         << "n5 = " << n5 << endl;
}

struct Foo{
    void pirnt_num(int n1, int n2)
    {
        cout << n1 + n2 << endl;
    }
    int data = 10;
};

int main(void)
{
    int n = 7;
    auto f1 = bind(f, _2, _1, 42, std::cref(n), n);
    n = 10;
    f1(1, 2, 101);

    Foo foo;
    auto f2 = bind(&Foo::pirnt_num, &foo, 95, _1);
    f2(5);

    return 0;
}
