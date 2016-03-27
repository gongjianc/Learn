/*
** @file    istream_iterator.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-09 18:10:29
*/
 
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
 
using std::cout;
using std::endl;
using std::cin;
using std::vector;
using std::istream_iterator;
using std::back_inserter;

int main(void)
{
    vector<int> vecInt;
    istream_iterator<int, char> isi(cin);
    std::copy(isi, istream_iterator<int, char>(), back_inserter(vecInt));
    vector<int>::iterator vit;
    for(vit = vecInt.begin(); vit != vecInt.end(); ++vit)
        cout << *vit << " ";

    cout << endl;
    return 0;
}

