/*
** @file    algorithm.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-09 18:51:12
*/
 
#include <iostream>
#include <algorithm>
#include <vector>
 
using std::cout;
using std::endl;
using std::vector;

void print(vector<int>::value_type value)
{
    //++value;
    cout << value << " ";
}
int main(void)
{
    int arr[6] = {1, 2, 3, 4, 5, 6};
    vector<int> vecInt(arr, arr + 6);
    for_each(vecInt.begin(), vecInt.end(), print);
    cout << endl;

    //c++11 的用法
    for(auto value : vecInt)
        cout << value << " ";

    return 0;
}
