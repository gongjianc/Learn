/*
** @file    associate_container.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-04 05:06:41
*/
 
#include <iostream>
#include <set>
 
using std::cout;
using std::endl;
using std::set;

void display(set<int> &setInt)
{
    set<int>::const_iterator cit;
    for(cit = setInt.cbegin(); cit != setInt.cend(); ++cit)
        cout << *cit << " ";
    cout << endl;
}
int main(void)
{
    int arr[9] = {2, 1, 3, 5, 4, 6, 3, 5, 6};
    set<int> setInt(arr, arr + 9);//set里边的元素不能重复，默认情况下，从下到大排序
    display(setInt);
    return 0;
}
