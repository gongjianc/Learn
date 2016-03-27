/*
** @file    reverse_iterator.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-10 00:41:35
*/
 
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
 
using std::cout;
using std::endl;
using std::vector;
using std::ostream_iterator;

int main(void)
{
    int arr[5] = {1, 2, 3, 4, 5};
    vector<int> vecInt(arr, arr + 5);
    ostream_iterator<int, char> osi(cout, " ");

    vector<int>::reverse_iterator ri = vecInt.rbegin();
    copy(ri, vecInt.rend(), osi);//reverse end;

    cout << endl;
    return 0;
}
