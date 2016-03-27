/*
** @file    unique_ptr.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-11 01:13:11
*/
 
#include <iostream>
#include <memory>
 
using std::cout;
using std::endl;
using std::unique_ptr;

int main(void)
{
    unique_ptr<int> upi(new int(88));
    cout << "*upi = " << *upi << endl;

    //unique_ptr<int> oen(upi); //不能复制，具有对象语义

    return 0;
}
