/*
** @file    algorithm2.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-09 19:05:24
*/
 
#include <iostream>
#include <algorithm>
#include <vector>
 
using std::cout;
using std::endl;
using std::vector;
//using std::replace_if;

void print(vector<int>::value_type value)
{
    cout << value << " ";
}

int main(void)
{
    int arr[6] = {1, 2, 3, 4, 5, 6};
    vector<int> vecInt(arr, arr + 6);
    for_each(vecInt.begin(), vecInt.end(), print);
    cout << endl;
    std::less<int> lt;
    replace_if(vecInt.begin(), vecInt.end(), std::bind2nd(lt, 3), 7);
    for_each(vecInt.begin(), vecInt.end(), print);
    cout << endl;

    return 0;
}
