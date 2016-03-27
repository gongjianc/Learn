/*
** @file    insert_iterator.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-10 00:50:23
*/
 
#include <iostream>
#include <vector>
 
using std::cout;
using std::endl;
using std::vector;

template <typename T>
void printElements(T c)
{
    typename T::const_iterator cit = c.begin();
    for(; cit != c.end(); ++cit)
        cout << *cit << " ";
    cout << endl;
}



