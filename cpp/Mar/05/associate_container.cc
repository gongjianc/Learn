/*
** @file    associate_container.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-04 19:05:01
*/
 
#include <iostream>
#include <map>
#include <string>
 
using std::cout;
using std::endl;
using std::string;
using std::map;
using std::pair;

int main(void)
{
    pair<const int, string> p1(600036, "招商银行");
    cout << p1.first << "-> " << p1.second << endl;
    return 0;
}
