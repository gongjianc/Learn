/*
** @file    stream_iterator.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-09 18:03:49
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
    int arr[6] = {1, 2, 3, 4, 5, 6};
    vector<int> vecInt(arr, arr + 6);

    //把输出流看成一个容器
    ostream_iterator<int, char> osi(cout, "\n");
    std::copy(vecInt.begin(), vecInt.end(), osi);

    cout << endl;

    return 0;

}
