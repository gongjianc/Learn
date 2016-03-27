/*
** @file    auto_ptr.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-11 00:27:14
*/
 
#include <iostream>
#include <memory>
 
using std::cout;
using std::endl;
using std::auto_ptr;

int main(void)
{
    double *pd = new double(7.77);
    auto_ptr<double> apd(pd);
    cout << "*apd = " << *apd << endl;
    cout << "*pd = " << *pd << endl;

    int *pi = new int(7);
    auto_ptr<int> api_1(pi);
    auto_ptr<int> api_2(api_1);//所有权的转移 
    cout << "*api_2 = " << *api_2 << endl;
    cout << "*api_1 = " << *api_1 << endl;
    return 0;
}

