/*
** @file    vector.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-11 04:28:27
*/
 
#include <iostream>
#include <vector>
 
using std::cout;
using std::endl;
using std::vector;

int main(void)
{
    vector<int> vecInt{1, 2, 3, 4, 5, 6, 7, 8 ,9};
    for(auto &i : vecInt)
        i *= i;
    for(auto i : vecInt)
        cout << i << " ";
    cout << endl;

    return 0;
}

