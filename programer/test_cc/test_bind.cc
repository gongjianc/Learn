/*
** @file    test_bind.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-19 00:45:29
*/

#include <iostream>
#include <functional>
using namespace std;

int TestFunc(int a, char c, float f)
{
    cout << a << endl;
    cout << c << endl;
    cout << f << endl;

    return a;
}

int main()
{
    auto bindFunc1 = bind(TestFunc, std::placeholders::_1, 'A', 100.1);
    bindFunc1(10);
    //bindFunc1(10, 'C');
    bindFunc1('C', 10);

    cout << "--------------" << endl;
    auto bindFunc2 = bind(TestFunc, std::placeholders::_2, std::placeholders::_1, 100.1);
    bindFunc2('B', 10);

    cout << "--------------" << endl;

    auto bindFunc3 = bind(TestFunc, std::placeholders::_2, std::placeholders::_3, std::placeholders::_1);
    bindFunc3(100.1, 30, 'C');
    cout << "--------------" << endl;


    return 0;
}

