/*
** @file    rand_for.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-11 04:49:43
*/
 
#include <iostream>
#include <string>
 
using std::cout;
using std::endl;
using std::string;

int main(void)
{
    string str("some string");
    for(auto c : str)
        cout << c << endl;

    return 0;
}
